#pragma once

#include "Common.h"
#include "Fmt/BaseFormatter.h"

#include <memory>
#include <mutex>

namespace FZXLog::Sink {

class BaseSink {
protected:

    // Properties

    std::shared_ptr<FZXLog::Fmt::BaseFormatter> m_formatter;
    Level m_level;
    Level m_flushLevel;

    // Methods

    virtual void write(
        const SourceLocation& p_loc,
        const Level& p_level,
        const std::string& p_message,
        const std::chrono::system_clock::time_point& p_timestamp,
        const std::thread::id& p_threadId
    ) = 0;

public:

    // Constructor/Destructor

    explicit BaseSink(
        std::shared_ptr<FZXLog::Fmt::BaseFormatter> p_formatter = nullptr,
        const Level& p_minLevel = Level::Trace,
        const Level& p_flushLevel = Level::Error
    ) :
        m_formatter(p_formatter),
        m_level(p_minLevel),
        m_flushLevel(p_flushLevel)
    {}
    virtual ~BaseSink() = default;

    // Methods

    void setMinLevel(const Level& p_level) {
        m_level = p_level;
    }
    Level getMinLevel() const {
        return m_level;
    }

    void setFormatter(std::shared_ptr<FZXLog::Fmt::BaseFormatter> p_formatter) {
        m_formatter = std::move(p_formatter);
    }
    std::shared_ptr<FZXLog::Fmt::BaseFormatter> getFormatter() const {
        return m_formatter;
    }

    void flushOn(const Level& p_level) {
        m_flushLevel = p_level;
    }

    void log(
        const SourceLocation& p_loc,
        const Level& p_level,
        const std::string& p_message,
        const std::chrono::system_clock::time_point& p_timestamp = std::chrono::system_clock::now(),
        const std::thread::id& p_threadId = std::this_thread::get_id()
    ) {
        if (static_cast<uint8_t>(p_level) < static_cast<uint8_t>(m_level) && p_level == Level::Off)
            return;
        write(
            p_loc,
            p_level,
            p_message,
            p_timestamp,
            p_threadId
        );
        if (static_cast<uint8_t>(p_level) >= static_cast<uint8_t>(m_flushLevel))
            flush();
    }

    void log(
        const Level& p_level,
        const std::string& p_message,
        const std::chrono::system_clock::time_point& p_timestamp = std::chrono::system_clock::now(),
        const std::thread::id& p_threadId = std::this_thread::get_id()
    ) {
        if (static_cast<uint8_t>(p_level) < static_cast<uint8_t>(m_level) && p_level == Level::Off)
            return;
        write(
            SourceLocation(),
            p_level,
            p_message,
            p_timestamp,
            p_threadId
        );
        if (static_cast<uint8_t>(p_level) >= static_cast<uint8_t>(m_flushLevel))
            flush();
    }

    virtual void flush() = 0;
};

} // namespace FZXLog::Sink
