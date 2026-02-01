#pragma once

#include "../Common.h"
#include "../Sink/Sink.h"

#include <unordered_set>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

// For variadic/format logging
#include <sstream>
#include <format>

namespace FZXLog::Logger {

class SyncLogger {
private:
    std::unordered_set<std::shared_ptr<Sink::BaseSink>> m_sinks;
    mutable std::mutex m_mutex;
    Level m_level;
    Level m_flushLevel;

public:
    SyncLogger(
        const Level& p_level = Level::Trace,
        const Level& p_flushLevel = Level::Error
    ) :
        m_level(p_level),
        m_flushLevel(p_flushLevel)
    {}
    ~SyncLogger();

    // Getters/Setters
    void setLevel(const Level& p_level) {
        m_level = p_level;
    }
    void flushOn(const Level& p_level) {
        m_flushLevel = p_level;
    }

    // Sink management
    void addSink(std::shared_ptr<FZXLog::Sink::BaseSink> p_sink);
    void removeSink(std::shared_ptr<FZXLog::Sink::BaseSink> p_sink);
    void clearSinks();

    // Raw log
    void log(const SourceLocation& p_loc, const Level& p_level, const std::string& p_message);
    inline void log(const Level& p_level, const std::string& p_message) { log(SourceLocation(), p_level, p_message); }

    // Variadic log
    template<typename... Args>
    void logv(const SourceLocation& p_loc, const Level& p_level, Args&&... p_args) {
        if (p_level == Level::Off)
            return;
        
        std::ostringstream oss;
        (oss << ... << std::forward<Args>(p_args));
        log(p_loc, p_level, oss.str());
    }

    template<typename... Args> inline void logv(const Level& p_level, Args&&... p_args) { logv(SourceLocation(), p_level, std::forward<Args>(p_args)...); }

    // Format log
    template<typename... Args>
    void logf(const SourceLocation& p_loc, const Level& p_level, std::format_string<Args...> p_fmtStr, Args&&... p_args) {
        if (p_level == Level::Off)
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

    // Flush sinks
    void flush();

};

} // namespace FZXLog::Logger
