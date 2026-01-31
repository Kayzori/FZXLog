#pragma once

#include "Log.h"
#include "FMT/BaseFormatter.h"

#include <memory>
#include <mutex>
#include <thread>
#include <chrono>

namespace FZXLog::Sink {

class BaseSink {
protected:
    std::shared_ptr<FZXLog::FMT::BaseFormatter> m_formatter;
    Level m_minLevel;

    // Write Func
    virtual void write(
        const Level& p_level,
        const std::string& p_message,
        const char* p_file = nullptr,
        const int p_line = 0,
        const char* p_func = nullptr,
        const std::chrono::system_clock::time_point& p_timestamp = std::chrono::system_clock::now(),
        const std::thread::id& p_threadId = std::this_thread::get_id()
    ) = 0;

public:
    explicit BaseSink(
        std::shared_ptr<FZXLog::FMT::BaseFormatter> p_formatter = nullptr,
        const Level& p_minLevel = Level::Trace
    ) :
        m_formatter(p_formatter),
        m_minLevel(p_minLevel)
    {}

    void setMinLevel(const Level& p_level) { m_minLevel = p_level; }
    Level getMinLevel() const { return m_minLevel; }

    virtual ~BaseSink() = default;

    // Log: Checks and Write
    void log(
        const Level& p_level,
        const std::string& p_message,
        const char* p_file = nullptr,
        const int p_line = 0,
        const char* p_func = nullptr,
        const std::chrono::system_clock::time_point& p_timestamp = std::chrono::system_clock::now(),
        const std::thread::id& p_threadId = std::this_thread::get_id()
    ) {
        if (static_cast<uint8_t>(p_level) >= static_cast<uint8_t>(m_minLevel) && p_level != Level::Off)
            write(
                p_level,
                p_message,
                p_file,
                p_line,
                p_func,
                p_timestamp,
                p_threadId
            );
    }

    // Optional flush hook
    virtual void flush() {}
};

} // namespace FZXLog::Sink
