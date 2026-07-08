#pragma once

#include "FZXLog/Utils.h"

#include <string>
#include <chrono>
#include <thread>
#include <stdint.h>


namespace FZXLog::Fmt {

// Base Abstract Formatter class
class Formatter {
public:
    // Constructor/Destructor

    Formatter() = default;
    virtual ~Formatter() = default;
    
    // Methods

    virtual std::string format(
        const SourceLocation& p_location,
        const Level& p_level,
        const std::string& p_message,
        const std::chrono::system_clock::time_point& p_timestamp,
        const std::thread::id& p_thread_id
    ) const noexcept = 0;

};

} // namespace FZXLog::Fmt
