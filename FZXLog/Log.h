#pragma once

#include <string>
#include <cstdint>

#define FZXLOG_ANSI_TRACE   "\x1b[38;2;127;127;127m"
#define FZXLOG_ANSI_DEBUG   "\x1b[38;2;0;255;255m"
#define FZXLOG_ANSI_INFO    "\x1b[38;2;255;255;255m"
#define FZXLOG_ANSI_SUCCESS "\x1b[38;2;0;255;0m"
#define FZXLOG_ANSI_WARNING "\x1b[38;2;255;255;0m"
#define FZXLOG_ANSI_ERROR   "\x1b[38;2;255;0;0m"
#define FZXLOG_ANSI_FATAL   "\x1b[38;2;255;0;255m"

#define FZXLOG_ANSI_RESET   "\x1b[0m"

namespace FZXLog {

enum class Level : uint8_t {
    Trace    = 0,
    Debug    = 1,
    Info     = 2,
    Success  = 3,
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
        case Level::Success: return "Success";
        case Level::Warning: return "Warning";
        case Level::Error:   return "Error";
        case Level::Fatal:   return "Fatal";
        default:             return "Unknown";
    }
}

// ANSI color codes
inline const char* getANSIColor(Level p_level) {
    switch (p_level) {
        case Level::Trace:       return FZXLOG_ANSI_TRACE;      // DarkGrey
        case Level::Debug:       return FZXLOG_ANSI_DEBUG;      // Cyan
        case Level::Info:        return FZXLOG_ANSI_INFO;       // White
        case Level::Success:     return FZXLOG_ANSI_SUCCESS;    // Green
        case Level::Warning:     return FZXLOG_ANSI_WARNING;    // Yellow
        case Level::Error:       return FZXLOG_ANSI_ERROR;      // Red
        case Level::Fatal:       return FZXLOG_ANSI_FATAL;      // Megenta
        default:                 return FZXLOG_ANSI_RESET;      // Default
    }
}

} // namespace FZXLog
