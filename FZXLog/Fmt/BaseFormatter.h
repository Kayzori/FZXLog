#pragma once

#include "Common.h"

#include <string>
#include <chrono>
#include <thread>

namespace FZXLog::Fmt {

class BaseFormatter {
public:

    // Constructor/Destructor

    BaseFormatter() = default;
    
    virtual ~BaseFormatter() = default;

    // Methods

    virtual std::string format(
        const Level& p_level,
        const std::string& p_message,
        const char* p_file,
        const int p_line,
        const char* p_func,
        const std::chrono::system_clock::time_point& p_timestamp,
        const std::thread::id& p_threadId
    ) = 0;

};

} // namespace FZXLog::Fmt
