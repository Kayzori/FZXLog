#pragma once

#include "Log.h"

#include <string>
#include <chrono>
#include <thread>

// Flag Macros

#define FZXLOG_FMT_COLOR            0b00000001
#define FZXLOG_FMT_LEVEL            0b00000010
#define FZXLOG_FMT_DATE             0b00000100
#define FZXLOG_FMT_TIME             0b00001000
#define FZXLOG_FMT_MS               0b00010000
#define FZXLOG_FMT_THREAD           0b00100000
#define FZXLOG_FMT_FILELINE         0b01000000
#define FZXLOG_FMT_FUNC             0b10000000

#define FZXLOG_FMT_BASIC_FLAGS      0b00001111
#define FZXLOG_FMT_ADVENCED_FLAGS   0b00111111
#define FZXLOG_FMT_FULL_FLAGS       0b11111111

namespace FZXLog::FMT {

class BaseFormatter {
protected:
    uint8_t m_flags;
public:
    void setFlags(const uint8_t p_flags) {
        m_flags = p_flags;
    }
    explicit BaseFormatter(const uint8_t p_flags = FZXLOG_FMT_BASIC_FLAGS) :
        m_flags(p_flags)
    {}
    
    virtual ~BaseFormatter() = default;
    virtual std::string format(
        const Level& p_level,
        const std::string& p_message,
        const char* p_file,
        const int p_line,
        const char* p_func,
        const std::chrono::system_clock::time_point& p_timestamp,
        const std::thread::id& p_threadId,
        const bool p_ANSIColorCode = false
    ) = 0;
};

} // namespace FZXLog::Fmt
