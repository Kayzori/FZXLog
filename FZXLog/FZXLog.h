#pragma once

#include "Common.h"
#include "Fmt/Fmt.h"
#include "Sink/Sink.h"
#include "Logger/AsyncLogger.h"

#include <iostream>

// Build-In
namespace FZXLog
{

static Logger::AsyncLogger* _globalLogger = nullptr;
static std::shared_ptr<FZXLog::Fmt::PatternFormatter> _defaultFormatter = std::make_shared<FZXLog::Fmt::PatternFormatter>();

// Initialization/Destruction

inline void initialize(const Level& p_level = Level::Trace, const Level& p_flushLevel = Level::Error) {
    _globalLogger = new Logger::AsyncLogger(p_level, p_flushLevel);
    _globalLogger->addSink(std::make_shared<FZXLog::Sink::ConsoleSink>(_defaultFormatter, true));
    _globalLogger->addSink(std::make_shared<FZXLog::Sink::RotationFileSink>("../../Logs/FZXEngine.log", _defaultFormatter));
}

inline bool isInitialized() {
    return _globalLogger != nullptr;
}

inline void shutdown() {
    if (_globalLogger) {
        delete _globalLogger;
        _globalLogger = nullptr;
    }
}

// Flags

inline void setLevel(const Level& p_level) {
    if (_globalLogger)
        _globalLogger->setLevel(p_level);
    else 
        std::cout << "Logger::AsyncLogger not initialized!" << std::endl;
}

inline void flushOn(const Level& p_level) {
    if (_globalLogger)
        _globalLogger->flushOn(p_level);
    else 
        std::cout << "Logger::AsyncLogger not initialized!" << std::endl;
}

inline void setPattern(const std::string& p_pattern) {
    if (_globalLogger)
        _defaultFormatter->setPattern(p_pattern);
    else 
        std::cout << "Logger::AsyncLogger not initialized!" << std::endl;
}

// Flush
inline void flush() {
    if (_globalLogger)
        _globalLogger->flush();
    else 
        std::cout << "Logger::AsyncLogger not initialized!" << std::endl;
}

// Logging

inline void log(const SourceLocation& p_loc, const Level& p_level, const std::string& p_message) {
    if (_globalLogger)
        _globalLogger->log(p_loc, p_level, p_message);
    else 
        std::cout << "Logger::AsyncLogger not initialized!" << std::endl;
}

inline void log(const Level& p_level, const std::string& p_message) {
    if (_globalLogger)
        _globalLogger->log(p_level, p_message);
    else 
        std::cout << "Logger::AsyncLogger not initialized!" << std::endl;
}

// Variadic log
template<typename... Args>
inline void logv(const SourceLocation& p_loc, const Level& p_level, Args&&... p_args) {
    if (_globalLogger)
        _globalLogger->logv(p_loc, p_level, std::forward<Args>(p_args)...);
    else 
        std::cout << "Logger::AsyncLogger not initialized!" << std::endl;
}

template<typename... Args>
inline void logv(const Level& p_level, Args&&... p_args) {
    if (_globalLogger)
        _globalLogger->logv(p_level, std::forward<Args>(p_args)...);
    else 
        std::cout << "Logger::AsyncLogger not initialized!" << std::endl;
}

// Format log

template<typename... Args>
inline void logf(const SourceLocation& p_loc, const Level& p_level, std::format_string<Args...> p_fmtStr, Args&&... p_args) {
    if (_globalLogger)
        _globalLogger->logf(p_loc, p_level, p_fmtStr, std::forward<Args>(p_args)...);
    else 
        std::cout << "Logger::AsyncLogger not initialized!" << std::endl;
}

template<typename... Args>
inline void logf(const Level& p_level, std::format_string<Args...> p_fmtStr, Args&&... p_args) {
    if (_globalLogger)
        _globalLogger->logf(p_level, p_fmtStr, std::forward<Args>(p_args)...);
    else 
        std::cout << "Logger::AsyncLogger not initialized!" << std::endl;
}

// Level helpers

template<typename... Args> inline void trace(std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(Level::Trace, p_fmtStr, std::forward<Args>(p_args)...); }
template<typename... Args> inline void debug(std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(Level::Debug, p_fmtStr, std::forward<Args>(p_args)...); }
template<typename... Args> inline void info(std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(Level::Info, p_fmtStr, std::forward<Args>(p_args)...); }
template<typename... Args> inline void warning(std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(Level::Warning, p_fmtStr, std::forward<Args>(p_args)...); }
template<typename... Args> inline void error(std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(Level::Error, p_fmtStr, std::forward<Args>(p_args)...); }
template<typename... Args> inline void fatal(std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(Level::Fatal, p_fmtStr, std::forward<Args>(p_args)...); }

template<typename... Args> inline void trace(const SourceLocation& p_loc, std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(p_loc, Level::Trace, p_fmtStr, std::forward<Args>(p_args)...); }
template<typename... Args> inline void debug(const SourceLocation& p_loc, std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(p_loc, Level::Debug, p_fmtStr, std::forward<Args>(p_args)...); }
template<typename... Args> inline void info(const SourceLocation& p_loc, std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(p_loc, Level::Info, p_fmtStr, std::forward<Args>(p_args)...); }
template<typename... Args> inline void warning(const SourceLocation& p_loc, std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(p_loc, Level::Warning, p_fmtStr, std::forward<Args>(p_args)...); }
template<typename... Args> inline void error(const SourceLocation& p_loc, std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(p_loc, Level::Error, p_fmtStr, std::forward<Args>(p_args)...); }
template<typename... Args> inline void fatal(const SourceLocation& p_loc, std::format_string<Args...> p_fmtStr, Args&&... p_args) { logf(p_loc, Level::Fatal, p_fmtStr, std::forward<Args>(p_args)...); }

} // namespace FZXLog
