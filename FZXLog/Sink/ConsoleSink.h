#pragma once

#include "BaseSink.h"

/*
what to fix:
- Implement the write and flush methods for ConsoleSink_st.
- Ensure proper handling of color output in the console.
- Verify thread safety in ConsoleSink_mt.
- Add necessary includes for mutex and other standard libraries.
no default constructor for ConsoleSink_st:
- Provide a constructor that requires a formatter and other parameters.
*/

namespace FZXLog::Sink {

// Single-Thread (Base)

class ConsoleSink_st : public BaseSink {
protected:

    // Methods

    virtual void write(
        const SourceLocation& p_loc,
        const Level& p_level,
        const std::string& p_message,
        const std::chrono::system_clock::time_point& p_timestamp = std::chrono::system_clock::now(),
        const std::thread::id& p_threadId = std::this_thread::get_id()
    ) override;

public:

    // Properties

    bool color;

    // Constructor/Destructor

    ConsoleSink_st(
        std::shared_ptr<FZXLog::Fmt::BaseFormatter> p_formatter,
        bool p_color = true,
        const Level& p_minLevel = Level::Trace,
        const Level& p_flushLevel = Level::Error
    ) :
        color(p_color),
        BaseSink(
            p_formatter,
            p_minLevel,
            p_flushLevel
        )
    {}

    virtual ~ConsoleSink_st() = default;

    // Methods

    virtual void flush() override;
};


// Multi-Thread

class ConsoleSink_mt : public ConsoleSink_st {
private:

    // Properties

    mutable std::mutex m_mutex;

protected:

    // Methods

    void write(
        const SourceLocation& p_loc,
        const Level& p_level,
        const std::string& p_message,
        const std::chrono::system_clock::time_point& p_timestamp = std::chrono::system_clock::now(),
        const std::thread::id& p_threadId = std::this_thread::get_id()
    ) override {
        std::lock_guard<std::mutex> lk(m_mutex);
        ConsoleSink_st::write(
            p_loc,
            p_level,
            p_message,
            p_timestamp,
            p_threadId
        );
    }

public:

    // Constructor/Destructor

    ConsoleSink_mt(
        std::shared_ptr<FZXLog::Fmt::BaseFormatter> p_formatter,
        const bool p_color = true,
        const Level& p_minLevel = Level::Trace,
        const Level& p_flushLevel = Level::Error
    ) :
        ConsoleSink_st(
            p_formatter,
            p_color,
            p_minLevel,
            p_flushLevel
        )
    {}

    ~ConsoleSink_mt() = default;

    void flush() override {
        std::lock_guard<std::mutex> lk(m_mutex);
        ConsoleSink_st::flush();
    }
};

} // namespace FZXLog::Sink