#pragma once

#include <string>
#include <cstdint>
#include <chrono>
#include <thread>

#define FZXLOG_ANSICODE_TRACE   "\x1b[38;2;127;127;127m"    // Grey
#define FZXLOG_ANSICODE_DEBUG   "\x1b[38;2;0;255;255m"      // Cyan
#define FZXLOG_ANSICODE_INFO    "\x1b[38;2;0;255;0m"        // Green
#define FZXLOG_ANSICODE_WARNING "\x1b[38;2;255;255;0m"      // Yellow
#define FZXLOG_ANSICODE_ERROR   "\x1b[38;2;255;0;0m"        // Red
#define FZXLOG_ANSICODE_FATAL   "\x1b[38;2;255;0;255m"      // Purple

#define FZXLOG_ANSICODE_RESET   "\x1b[0m"                   // Default


namespace FZXLog {

enum class Level : uint8_t {
    Trace    = 0,
    Debug    = 1,
    Info     = 2,
    Warning  = 4,
    Error    = 5,
    Fatal    = 6,
    Off      = 7
};

inline const char* levelToString(Level p_level) {
    switch (p_level) {
        case Level::Trace:   return "Trace";
        case Level::Debug:   return "Debug";
        case Level::Info:    return "Info";
        case Level::Warning: return "Warning";
        case Level::Error:   return "Error";
        case Level::Fatal:   return "Fatal";
        default:             return "Unknown";
    }
}

// ANSI color codes
inline const char* getANSICode(Level p_level) {
    switch (p_level) {
        case Level::Trace:       return FZXLOG_ANSICODE_TRACE;
        case Level::Debug:       return FZXLOG_ANSICODE_DEBUG;
        case Level::Info:        return FZXLOG_ANSICODE_INFO;
        case Level::Warning:     return FZXLOG_ANSICODE_WARNING;
        case Level::Error:       return FZXLOG_ANSICODE_ERROR;
        case Level::Fatal:       return FZXLOG_ANSICODE_FATAL;
        default:                 return FZXLOG_ANSICODE_RESET;
    }
}

struct SourceLocation {

    // Properties

    const char* file;
    uint32_t line;
    const char* func;

    // Constructor/Destructor

    inline constexpr SourceLocation(
        const char* p_file = "",
        uint32_t p_line = -1,
        const char* p_func = ""
    ) :
        file(p_file),
        line(p_line),
        func(p_func)
    {}
    ~SourceLocation() = default;

    // Operators
    void operator=(const SourceLocation& other) {
        file = other.file;
        line = other.line;
        func = other.func;
    }
};

struct LogRecord {
    SourceLocation location;
    Level level;
    std::string message;
    std::chrono::system_clock::time_point timestamp;
    std::thread::id threadId;

    ~LogRecord() = default;

    void operator=(const LogRecord& other) {
        location = other.location;
        level = other.level;
        message = other.message;
        timestamp = other.timestamp;
        threadId = other.threadId;
    }
};

} // namespace FZXLog

#define FZXLOG_LEVEL_TRACE      FZXLog::Level::Trace
#define FZXLOG_LEVEL_DEBUG      FZXLog::Level::Debug
#define FZXLOG_LEVEL_INFO       FZXLog::Level::Info
#define FZXLOG_LEVEL_WARNING    FZXLog::Level::Warning
#define FZXLOG_LEVEL_ERROR      FZXLog::Level::Error
#define FZXLOG_LEVEL_FATAL      FZXLog::Level::Fatal
#define FZXLOG_LEVEL_OFF        FZXLog::Level::Off

#define FZXLOG_LOCATION \
    FZXLog::SourceLocation(__FILE__, __LINE__, __FUNCTION__)
