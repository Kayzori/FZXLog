#pragma once

#include "BaseFormatter.h"

// [yyyy-mm-dd hh:mm:ss.ms] [level] [thread:id] file:line function - message

namespace FZXLog::FMT {

class DefaultFormatter : public BaseFormatter {
public:
    explicit DefaultFormatter(const uint8_t p_flags) :
        BaseFormatter(p_flags)
    {}

    std::string format(
        const Level& p_level,
        const std::string& p_message,
        const char* p_file,
        const int p_line,
        const char* p_func,
        const std::chrono::system_clock::time_point& p_timestamp,
        const std::thread::id& p_threadId,
        const bool p_ANSIColor = false
    ) override;
};

} // namespace FZXLog::Fmt
