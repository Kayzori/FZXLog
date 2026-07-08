#pragma once

#include "FZXLog/Fmt/Formatter.h"

namespace FZXLog::Sink {

// Base Abstract Sink class
class Sink {
protected:

    // Private Members

    std::shared_ptr<FZXLog::Fmt::Formatter> m_formatter;
    Level m_level;
    Level m_flush_level;

    // Methods

    virtual void write(
        const SourceLocation& p_location,
        const Level& p_level,
        const std::string& p_message,
        const std::chrono::system_clock::time_point& p_timestamp = std::chrono::system_clock::now(),
        const std::thread::id& p_thread_id = std::this_thread::get_id()
    ) noexcept = 0;

public:

    // Constructor/Destructor

    explicit Sink(
        std::shared_ptr<FZXLog::Fmt::Formatter> p_formatter = nullptr,
        const Level& p_level = Level::Trace,
        const Level& p_flush_level = Level::Error
    ) noexcept :
        m_formatter(std::move(p_formatter)),
        m_level(p_level),
        m_flush_level(p_flush_level)
    {}
    virtual ~Sink() = default;

    // Methods

    void setMinLevel(const Level& p_level) noexcept {
        m_level = p_level;
    }
    Level getMinLevel() const noexcept {
        return m_level;
    }

    void setFlushLevel(const Level& p_flush_level) noexcept {
        m_flush_level = p_flush_level;
    }
    Level getFlushLevel() const noexcept {
        return m_flush_level;
    }

    void setFormatter(std::shared_ptr<FZXLog::Fmt::Formatter> p_formatter) noexcept {
        m_formatter = std::move(p_formatter);
    }
    std::shared_ptr<FZXLog::Fmt::Formatter> getFormatter() const noexcept {
        return m_formatter;
    }



    virtual void log(
        const SourceLocation& p_location,
        const FZXLog::Level& p_level,
        const std::string& p_message,
        const std::chrono::system_clock::time_point& p_timestamp = std::chrono::system_clock::now(),
        const std::thread::id& p_thread_id = std::this_thread::get_id()
    ) noexcept {
        if (static_cast<uint8_t>(p_level) < static_cast<uint8_t>(m_level) && p_level == Level::Off)
            return;

        write(p_location, p_level, p_message, p_timestamp, p_thread_id);

        if (static_cast<uint8_t>(p_level) >= static_cast<uint8_t>(m_flush_level))
            flush();
    }

    void log(
        const Level& p_level,
        const std::string& p_message,
        const std::chrono::system_clock::time_point& p_timestamp = std::chrono::system_clock::now(),
        const std::thread::id& p_threadId = std::this_thread::get_id()
    ) noexcept {
        if (static_cast<uint8_t>(p_level) < static_cast<uint8_t>(m_level) && p_level == Level::Off)
            return;
        write(
            SourceLocation(),
            p_level,
            p_message,
            p_timestamp,
            p_threadId
        );
        if (static_cast<uint8_t>(p_level) >= static_cast<uint8_t>(m_flush_level))
            flush();
    }

    virtual void flush() noexcept = 0;
};

} // namespace FZXLog::Sink
