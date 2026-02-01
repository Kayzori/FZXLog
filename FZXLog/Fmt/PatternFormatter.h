#pragma once

#include "BaseFormatter.h"

// Build-In pattern macros

#define FZXLOG_FMT_PATTERN_BASIC       "[%y-%m-%d %H:%M:%S] [%l] - %v"
#define FZXLOG_FMT_PATTERN_ADVENCED    "[%y-%m-%d %H:%M:%S.%e] [%l] [thread: %t] - %v"
#define FZXLOG_FMT_PATTERN_FULL        "[%y-%m-%d %H:%M:%S.%e] [%l] [thread: %t] <location: %s:%#> <function: %!> - %v"

namespace FZXLog::Fmt {

class PatternFormatter : public BaseFormatter {
private:

    // Properties

    std::string m_pattern;

public:

    // Constructor/Destructor

    explicit PatternFormatter(
        const std::string& p_pattern = FZXLOG_FMT_PATTERN_BASIC
    ) :
        m_pattern(p_pattern)
    {}

    // Methods

    void setPattern(const std::string& p_pattern) {
        m_pattern = p_pattern;
    }
    const std::string& getPattern() const {
        return m_pattern;
    }

    std::string format(
        const Level& p_level,
        const std::string& p_message,
        const char* p_file,
        const int p_line,
        const char* p_func,
        const std::chrono::system_clock::time_point& p_timestamp,
        const std::thread::id& p_threadId
    ) override;
};

} // namespace FZXLog::Fmt
