#pragma once

// Deprecated, use SyncLogger instead. Async logging is not recommended due to potential message loss on crashes and increased complexity.

#include "SyncLogger.h"
#include "FZXLog/Utils.h"

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace FZXLog::Logger {

class AsyncLogger : public SyncLogger {
private:
    std::queue<LogRecord> m_queue;
    std::mutex m_queueMutex;
    std::condition_variable m_cv;
    std::thread m_worker;
    std::atomic<bool> m_running{true};

    // Worker thread loop
    void workerLoop() {
        while (m_running || !m_queue.empty()) {
            LogRecord record{
                SourceLocation(),
                Level::Off,
                std::string(),
                std::chrono::system_clock::now(),
                std::thread::id()
            };
            {
                std::unique_lock lock(m_queueMutex);
                m_cv.wait(lock, [&]() { return !m_queue.empty() || !m_running; });

                if (m_queue.empty()) {
                    continue;
                }

                record = std::move(m_queue.front());
                m_queue.pop();
            }

            // Write to sinks using base SyncLogger implementation
            SyncLogger::log(record.m_location, record.m_level, record.m_message);
        }
        // final flush
        SyncLogger::flush();
    }

public:
    AsyncLogger(const AsyncLogger&) = delete;
    AsyncLogger& operator=(const AsyncLogger&) = delete;
    AsyncLogger(AsyncLogger&&) = delete;
    AsyncLogger& operator=(AsyncLogger&&) = delete;
    AsyncLogger(const Level& p_level = Level::Trace, const Level& p_flushLevel = Level::Error, const size_t& p_log_trace_capacity = 100)
        : SyncLogger(p_level, p_flushLevel, p_log_trace_capacity)
    {
        m_worker = std::thread(&AsyncLogger::workerLoop, this);
    }
    ~AsyncLogger() {
        m_running = false;
        m_cv.notify_all();
        if (m_worker.joinable())
            m_worker.join();
    }

    // Log a message
    void log(const SourceLocation& p_loc, const Level& p_level, const std::string& p_message) override {
        if (p_level == Level::Off || static_cast<uint8_t>(p_level) < static_cast<uint8_t>(getLevel()))
            return;

        LogRecord record{
            p_loc,
            p_level,
            p_message,
            std::chrono::system_clock::now(),
            std::this_thread::get_id()
        };

        {
            std::lock_guard lock(m_queueMutex);
            m_queue.push(std::move(record));
        }
        m_cv.notify_one();
    }
    void log(const Level& p_level, const std::string& p_message) override { log(SourceLocation(), p_level, p_message); }

    void flush() override {
        std::unique_lock lock(m_queueMutex);
        m_cv.wait(lock, [&]() { return m_queue.empty(); });
        SyncLogger::flush();
    }
};

} // namespace FZXLog::Logger