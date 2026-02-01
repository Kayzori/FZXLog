#pragma once

#include "SyncLogger.h"
#include "../Common.h"

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include <sstream>
#include <format>

namespace FZXLog::Logger {

class AsyncLogger {
private:
    SyncLogger m_syncLogger; // internal sync logger to write to sinks
    std::queue<LogRecord> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::thread m_worker;
    std::atomic<bool> m_running{true};

    Level m_level;
    Level m_flushLevel;

    // Worker thread loop
    void workerLoop() {
        while (m_running || !m_queue.empty()) {
            LogRecord record;
            {
                std::unique_lock lock(m_mutex);
                m_cv.wait(lock, [&]() { return !m_queue.empty() || !m_running; });

                if (!m_queue.empty()) {
                    record = std::move(m_queue.front());
                    m_queue.pop();
                } else {
                    continue;
                }
            }

            // Write to sinks using SyncLogger
            m_syncLogger.log(record.location, record.level, record.message);
        }
        // final flush
        m_syncLogger.flush();
    }

public:
    AsyncLogger(const Level& p_level = Level::Trace, const Level& p_flushLevel = Level::Error)
        :
        m_syncLogger(p_level, p_flushLevel),
        m_level(p_level),
        m_flushLevel(p_flushLevel)
    {
        m_worker = std::thread(&AsyncLogger::workerLoop, this);
    }
    ~AsyncLogger() {
        m_running = false;
        m_cv.notify_all();
        if (m_worker.joinable())
            m_worker.join();
    }

    // Getters/Setters
    void setLevel(const Level& p_level) {
        m_level = p_level;
        m_syncLogger.setLevel(p_level);
    }
    void flushOn(const Level& p_level) {
        m_flushLevel = p_level;
        m_syncLogger.flushOn(p_level);
    }

    // Sink management
    void addSink(std::shared_ptr<Sink::BaseSink> p_sink) {
        m_syncLogger.addSink(std::move(p_sink));
    }
    void removeSink(std::shared_ptr<Sink::BaseSink> p_sink) {
        m_syncLogger.removeSink(std::move(p_sink));
    }
    void clearSinks() {
        m_syncLogger.clearSinks();
    }

    // Log a message
    void log(const SourceLocation& p_loc, Level p_level, const std::string& p_message) {
        if (p_level == Level::Off || static_cast<uint8_t>(p_level) < static_cast<uint8_t>(m_level))
            return;

        LogRecord record{
            p_loc,
            p_level,
            p_message,
            std::chrono::system_clock::now(),
            std::this_thread::get_id()
        };

        {
            std::lock_guard lock(m_mutex);
            m_queue.push(std::move(record));
        }
        m_cv.notify_one();
    }
    inline void log(const Level& p_level, const std::string& p_message) { log(SourceLocation(), p_level, p_message); }

    // Variadic log
    template<typename... Args>
    void logv(const SourceLocation& p_loc, const Level& p_level, Args&&... p_args) {
        if (p_level == Level::Off || static_cast<uint8_t>(p_level) < static_cast<uint8_t>(m_level))
            return;
        
        std::ostringstream oss;
        (oss << ... << std::forward<Args>(p_args));
        log(p_loc, p_level, oss.str());
    }

    template<typename... Args> inline void logv(const Level& p_level, Args&&... p_args) { logv(SourceLocation(), p_level, std::forward<Args>(p_args)...); }

    // Format log
    template<typename... Args>
    void logf(const SourceLocation& p_loc, const Level& p_level, std::format_string<Args...> p_fmtStr, Args&&... p_args) {
        if (p_level == Level::Off || static_cast<uint8_t>(p_level) < static_cast<uint8_t>(m_level))
            return;

        log(p_loc, p_level, std::format(p_fmtStr, std::forward<Args>(p_args)...));
    }

    template<typename... Args> inline void logf(const Level& p_level, std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(SourceLocation(), p_level, p_fmtStr, std::forward<Args>(p_args)...); }


    // Level helpers
    template<typename... Args> inline void trace(std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(Level::Trace, p_fmtStr, std::forward<Args>(p_args)...); }
    template<typename... Args> inline void debug(std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(Level::Debug, p_fmtStr, std::forward<Args>(p_args)...); }
    template<typename... Args> inline void info(std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(Level::Info, p_fmtStr, std::forward<Args>(p_args)...); }
    template<typename... Args> inline void success(std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(Level::Success, p_fmtStr, std::forward<Args>(p_args)...); }
    template<typename... Args> inline void warning(std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(Level::Warning, p_fmtStr, std::forward<Args>(p_args)...); }
    template<typename... Args> inline void error(std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(Level::Error, p_fmtStr, std::forward<Args>(p_args)...); }
    template<typename... Args> inline void fatal(std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(Level::Fatal, p_fmtStr, std::forward<Args>(p_args)...); }
    
    template<typename... Args> inline void trace(const SourceLocation& p_loc, std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(p_loc, Level::Trace, p_fmtStr, std::forward<Args>(p_args)...); }
    template<typename... Args> inline void debug(const SourceLocation& p_loc, std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(p_loc, Level::Debug, p_fmtStr, std::forward<Args>(p_args)...); }
    template<typename... Args> inline void info(const SourceLocation& p_loc, std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(p_loc, Level::Info, p_fmtStr, std::forward<Args>(p_args)...); }
    template<typename... Args> inline void success(const SourceLocation& p_loc, std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(p_loc, Level::Success, p_fmtStr, std::forward<Args>(p_args)...); }
    template<typename... Args> inline void warning(const SourceLocation& p_loc, std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(p_loc, Level::Warning, p_fmtStr, std::forward<Args>(p_args)...); }
    template<typename... Args> inline void error(const SourceLocation& p_loc, std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(p_loc, Level::Error, p_fmtStr, std::forward<Args>(p_args)...); }
    template<typename... Args> inline void fatal(const SourceLocation& p_loc, std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(p_loc, Level::Fatal, p_fmtStr, std::forward<Args>(p_args)...); }

    // Flush logs
    void flush() {
        std::unique_lock lock(m_mutex);
        m_cv.wait(lock, [&]() { return m_queue.empty(); });
        m_syncLogger.flush();
    }

};

} // namespace FZXLog::Logger