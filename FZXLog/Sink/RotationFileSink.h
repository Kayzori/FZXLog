#pragma once

#include "BaseSink.h"

#include <fstream>

namespace FZXLog::Sink {

// Single-Threaded (Base)

class RotationFileSink_st : public BaseSink {
private:

    // Properties

    std::string m_filename;
    size_t m_size;
    size_t m_currentSize;
    int m_count;

    std::string m_buffer;
    size_t m_flushThreshold;
    std::ofstream m_ofs;

    // Methods

    void rotate();
    void open();
    void flushBuffer();

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

    // Constructor/Destructor

    RotationFileSink_st(
        const std::string& p_filename,
        std::shared_ptr<FZXLog::Fmt::BaseFormatter> p_formatter = nullptr,
        const Level& p_minLevel = Level::Trace,
        const Level& p_flushLevel = Level::Error,
        const size_t p_size = 1048576,
        const int p_count = 10,
        const size_t p_flushThreshold = 8192
    );

    virtual ~RotationFileSink_st() override;

    // Methods

    virtual void flush() override;
};


// Multi-Threaded

class RotationFileSink_mt : public RotationFileSink_st {
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
        RotationFileSink_st::write(
            p_loc,
            p_level,
            p_message,
            p_timestamp,
            p_threadId
        );
    }

public:

    // Constructor/Destructor

    RotationFileSink_mt(
        const std::string& p_filename,
        std::shared_ptr<FZXLog::Fmt::BaseFormatter> p_formatter,
        const Level& p_minLevel = Level::Trace,
        const Level& p_flushLevel = Level::Error,
        const size_t p_size = 1048576,
        const int p_count = 10
    ) :
        RotationFileSink_st(
            p_filename,
            p_formatter,
            p_minLevel,
            p_flushLevel,
            p_size,
            p_count
        )
    {}

    ~RotationFileSink_mt() = default;

    // Methods

    void flush() override {
        std::lock_guard<std::mutex> lk(m_mutex);
        RotationFileSink_st::flush();
    }

};

} // namespace FZXLog::Sink