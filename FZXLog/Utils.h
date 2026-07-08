#pragma once

#include <string>
#include <chrono>
#include <thread>
#include <stdint.h>

namespace FZXLog {

enum class Level : uint8_t {
    Trace   = 0,
    Debug   = 1,
    Info    = 2,
    Warning = 3,
    Error   = 4,
    Fatal   = 5,
    Off     = 6
};

static constexpr const char* FZXLogLevelToString(Level p_level) noexcept {
    switch (p_level) {
        case Level::Trace: return "Trace";
        case Level::Debug: return "Debug";
        case Level::Info: return "Info";
        case Level::Warning: return "Warning";
        case Level::Error: return "Error";
        case Level::Fatal: return "Fatal";
        case Level::Off: return "Off";
        default: return "Unknown";
    }
}

struct SourceLocation {

    // Public members

    const char* m_file;
    uint32_t m_line;
    const char* m_func;

    // Constructor/Destructor

    inline SourceLocation(
        const char* p_file = "",
        uint32_t p_line = -1,
        const char* p_func = ""
    ) noexcept :
        m_file(p_file),
        m_line(p_line),
        m_func(p_func)
    {}
    ~SourceLocation() = default;

    // Operators
    
    inline void operator=(const SourceLocation& p_other) noexcept {
        m_file = p_other.m_file;
        m_line = p_other.m_line;
        m_func = p_other.m_func;
    }
};

struct LogRecord {

    // Public members

    SourceLocation m_location;
    Level m_level;
    std::string m_message;
    std::chrono::system_clock::time_point m_timestamp;
    std::thread::id m_threadId;


    // Constructor/Destructor

    inline LogRecord(
        const SourceLocation& p_location,
        const Level& p_level,
        const std::string& p_message,
        const std::chrono::system_clock::time_point& p_timestamp,
        const std::thread::id& p_threadId
    ) noexcept :
        m_location(p_location),
        m_level(p_level),
        m_message(p_message),
        m_timestamp(p_timestamp),
        m_threadId(p_threadId)
    {}

    ~LogRecord() = default;

    // Operators

    inline void operator=(const LogRecord& other) noexcept {
        m_location = other.m_location;
        m_level = other.m_level;
        m_message = other.m_message;
        m_timestamp = other.m_timestamp;
        m_threadId = other.m_threadId;
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

#define FZXLOG_SOURCE_LOCATION \
    FZXLog::SourceLocation(__FILE__, __LINE__, __FUNCTION__)
