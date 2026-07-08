#pragma once

#include "Formatter.h"

// Pattern Types

#define FZXLOG_FMT_PATTERN_BASIC       "[%y-%m-%d %H:%M:%S] [%l] - %v"
#define FZXLOG_FMT_PATTERN_ADVENCED    "[%y-%m-%d %H:%M:%S.%e] [%l] [thread: %t] - %v"
#define FZXLOG_FMT_PATTERN_FULL        "[%y-%m-%d %H:%M:%S.%e] [%l] [thread: %t] <location: %s:%#> <function: %!> - %v"

namespace FZXLog::Fmt {

class PatternFormatter : public FZXLog::Fmt::Formatter {
private:
    // Private Members

    std::string m_pattern;

public:

    // Constructor/Destructor

    PatternFormatter(const std::string& p_pattern = FZXLOG_FMT_PATTERN_BASIC) noexcept :
        m_pattern(p_pattern)
    {}
    ~PatternFormatter() override = default;
    
    // Methods

    std::string format(
        const SourceLocation& p_location,
        const FZXLog::Level& p_level,
        const std::string& p_message,
        const std::chrono::system_clock::time_point& p_timestamp,
        const std::thread::id& p_thread_id
    ) const noexcept override;

};

} // namespace FZXLog::Fmt