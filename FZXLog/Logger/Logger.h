#pragma once

#include "FZXLog/Utils.h"
#include "FZXLog/Sink/Sink.h"

#include <format>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

namespace FZXLog::Logger {

// Base Logger class
class Logger {
protected:
    std::unordered_set<std::shared_ptr<Sink::Sink>> m_sinks;
    Level m_level;
    Level m_flushLevel;
    std::vector<LogRecord> m_log_trace;
    size_t m_log_trace_capacity;

public:
    Logger(
        const Level& p_level = Level::Trace,
        const Level& p_flushLevel = Level::Error,
        const size_t& p_log_trace_capacity = 100
    ) :
        m_level(p_level),
        m_flushLevel(p_flushLevel),
        m_log_trace_capacity(p_log_trace_capacity)
    {}

    virtual ~Logger() = default;

    virtual void setLevel(const Level& p_level) {
        m_level = p_level;
    }
    virtual Level getLevel() const {
        return m_level;
    }

    virtual void setFlushLevel(const Level& p_level) {
        m_flushLevel = p_level;
    }
    virtual Level getFlushLevel() const {
        return m_flushLevel;
    }

    virtual void addSink(std::shared_ptr<FZXLog::Sink::Sink> p_sink) {
        m_sinks.emplace(std::move(p_sink));
    }
    virtual void removeSink(std::shared_ptr<FZXLog::Sink::Sink> p_sink) {
        m_sinks.erase(std::move(p_sink));
    }
    virtual void clearSinks() {
        m_sinks.clear();
    }
    virtual std::vector<std::shared_ptr<FZXLog::Sink::Sink>> getSinks() const {
        return std::vector<std::shared_ptr<Sink::Sink>>(m_sinks.begin(), m_sinks.end());
    }

    virtual void log(const SourceLocation& p_loc, const Level& p_level, const std::string& p_message);
    virtual void log(const Level& p_level, const std::string& p_message) {
        log(SourceLocation(), p_level, p_message);
    }

    virtual void flush() = 0;
    virtual std::vector<LogRecord> getLogTrace() const {
        return m_log_trace;
    }
    virtual void setLogTraceCapacity(const size_t& p_capacity) {
        m_log_trace_capacity = p_capacity;
        while (m_log_trace.size() > m_log_trace_capacity) {
            m_log_trace.erase(m_log_trace.begin());
        }
    }
    virtual size_t getLogTraceCapacity() const {
        return m_log_trace_capacity;
    }

    template<typename... Args>
    void logv(const SourceLocation& p_loc, const Level& p_level, Args&&... p_args) {
        if (p_level == Level::Off || static_cast<uint8_t>(p_level) < static_cast<uint8_t>(m_level))
            return;

        std::ostringstream oss;
        (oss << ... << std::forward<Args>(p_args));
        log(p_loc, p_level, oss.str());
    }

    template<typename... Args>
    void logv(const Level& p_level, Args&&... p_args) {
        logv(SourceLocation(), p_level, std::forward<Args>(p_args)...);
    }

    template<typename... Args>
    void logf(const SourceLocation& p_loc, const Level& p_level, std::format_string<Args...> p_fmtStr, Args&&... p_args) {
        if (p_level == Level::Off || static_cast<uint8_t>(p_level) < static_cast<uint8_t>(m_level))
            return;

        log(p_loc, p_level, std::format(p_fmtStr, std::forward<Args>(p_args)...));
    }

    template<typename... Args>
    void logf(const Level& p_level, std::format_string<Args...> p_fmtStr, Args&&... p_args) {
        logf(SourceLocation(), p_level, p_fmtStr, std::forward<Args>(p_args)...);
    }

    template<typename... Args> void trace(std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(Level::Trace, p_fmtStr, std::forward<Args>(p_args)...); }
    template<typename... Args> void debug(std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(Level::Debug, p_fmtStr, std::forward<Args>(p_args)...); }
    template<typename... Args> void info(std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(Level::Info, p_fmtStr, std::forward<Args>(p_args)...); }
    template<typename... Args> void warning(std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(Level::Warning, p_fmtStr, std::forward<Args>(p_args)...); }
    template<typename... Args> void error(std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(Level::Error, p_fmtStr, std::forward<Args>(p_args)...); }
    template<typename... Args> void fatal(std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(Level::Fatal, p_fmtStr, std::forward<Args>(p_args)...); }

    template<typename... Args> void trace(const SourceLocation& p_loc, std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(p_loc, Level::Trace, p_fmtStr, std::forward<Args>(p_args)...); }
    template<typename... Args> void debug(const SourceLocation& p_loc, std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(p_loc, Level::Debug, p_fmtStr, std::forward<Args>(p_args)...); }
    template<typename... Args> void info(const SourceLocation& p_loc, std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(p_loc, Level::Info, p_fmtStr, std::forward<Args>(p_args)...); }
    template<typename... Args> void warning(const SourceLocation& p_loc, std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(p_loc, Level::Warning, p_fmtStr, std::forward<Args>(p_args)...); }
    template<typename... Args> void error(const SourceLocation& p_loc, std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(p_loc, Level::Error, p_fmtStr, std::forward<Args>(p_args)...); }
    template<typename... Args> void fatal(const SourceLocation& p_loc, std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(p_loc, Level::Fatal, p_fmtStr, std::forward<Args>(p_args)...); }
};

} // namespace FZXLog::Logger
