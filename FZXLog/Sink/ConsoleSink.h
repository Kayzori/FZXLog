#pragma once

#include "Sink.h"

#include <mutex>

// Color Configuration

#define FZXLOG_ANSICODE_TRACE   "\x1b[38;2;127;127;127m"    // Grey
#define FZXLOG_ANSICODE_DEBUG   "\x1b[38;2;0;255;255m"      // Cyan
#define FZXLOG_ANSICODE_INFO    "\x1b[38;2;0;255;0m"        // Green
#define FZXLOG_ANSICODE_WARNING "\x1b[38;2;255;255;0m"      // Yellow
#define FZXLOG_ANSICODE_ERROR   "\x1b[38;2;255;0;0m"        // Red
#define FZXLOG_ANSICODE_FATAL   "\x1b[38;2;255;0;255m"      // Purple

#define FZXLOG_ANSICODE_RESET   "\x1b[0m"                   // Default

namespace FZXLog {

static inline const char* FZXLogLevelToAnsiCode(Level p_level) {
    switch (p_level) {
        case Level::Trace:       return FZXLOG_ANSICODE_TRACE;
        case Level::Debug:       return FZXLOG_ANSICODE_DEBUG;
        case Level::Info:        return FZXLOG_ANSICODE_INFO;
        case Level::Warning:     return FZXLOG_ANSICODE_WARNING;
        case Level::Error:       return FZXLOG_ANSICODE_ERROR;
        case Level::Fatal:       return FZXLOG_ANSICODE_FATAL;
        default:                 return FZXLOG_ANSICODE_RESET;
    }
}

}



namespace FZXLog::Sink {

class ConsoleSink_st : public Sink {
private:

    // Private members

    bool m_colored;

protected:

    // Methods

    virtual void write(
        const SourceLocation& p_loc,
        const Level& p_level,
        const std::string& p_message,
        const std::chrono::system_clock::time_point& p_timestamp = std::chrono::system_clock::now(),
        const std::thread::id& p_threadId = std::this_thread::get_id()
    ) noexcept override;

public:

    // Constructor/Destructor

    ConsoleSink_st(
        std::shared_ptr<FZXLog::Fmt::Formatter> p_formatter,
        const Level& p_minLevel = Level::Trace,
        const Level& p_flushLevel = Level::Error,
        bool p_colored = true
    ) noexcept :
        m_colored(p_colored),
        Sink(
            p_formatter,
            p_minLevel,
            p_flushLevel
        )
    {}
    virtual ~ConsoleSink_st() = default;

    // Methods

    virtual void flush() noexcept override;
};

class ConsoleSink_mt : public ConsoleSink_st {
private:

    // Mutex for thread safety

    mutable std::mutex m_mutex;

protected:

    // Methods

    virtual void write(
        const SourceLocation& p_loc,
        const Level& p_level,
        const std::string& p_message,
        const std::chrono::system_clock::time_point& p_timestamp = std::chrono::system_clock::now(),
        const std::thread::id& p_threadId = std::this_thread::get_id()
    ) noexcept override {
        std::lock_guard<std::mutex> lock(m_mutex);
        ConsoleSink_st::write(p_loc, p_level, p_message, p_timestamp, p_threadId);
    }

public:

    // Constructor/Destructor

    ConsoleSink_mt(
        std::shared_ptr<FZXLog::Fmt::Formatter> p_formatter,
        const Level& p_minLevel = Level::Trace,
        const Level& p_flushLevel = Level::Error,
        bool p_colored = true
    ) noexcept :
        ConsoleSink_st(
            p_formatter,
            p_minLevel,
            p_flushLevel,
            p_colored
        )
    {}
    virtual ~ConsoleSink_mt() = default;

    // Methods

    virtual void flush() noexcept override {
        std::lock_guard<std::mutex> lock(m_mutex);
        ConsoleSink_st::flush();
    }
};

} // namespace FZXLog::Sink
