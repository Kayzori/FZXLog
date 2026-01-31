#pragma once

#include "BaseSink.h"

#include <fstream>

namespace FZXLog::Sink {

// Single-Threaded
class RotationFileSink_st : public BaseSink {
private:
    std::string m_filename;
    size_t m_size;
    size_t m_currentSize;
    int m_count;

    std::string m_buffer;
    size_t m_flushThreshold;
    std::ofstream m_ofs;

    void rotate();
    void open();
    void flushBuffer();

protected:
    virtual void write(
        const Level& p_level,
        const std::string& p_message,
        const char* p_file = nullptr,
        const int p_line = 0,
        const char* p_func = nullptr,
        const std::chrono::system_clock::time_point& p_timestamp = std::chrono::system_clock::now(),
        const std::thread::id& p_threadId = std::this_thread::get_id()
    ) override;

public:
    RotationFileSink_st(
        const std::string& p_filename,
        std::shared_ptr<FZXLog::FMT::BaseFormatter> p_formatter = nullptr,
        const Level& p_minLevel = Level::Trace,
        const size_t p_size = 1048576,
        const int p_count = 10,
        const size_t p_flushThreshold = 8192
    );

    virtual ~RotationFileSink_st() override;

    virtual void flush() override;
};

// Multi-Threaded
class RotationFileSink_mt : public RotationFileSink_st {
private:
    mutable std::mutex m_mutex;

protected:
    void write(
        const Level& p_level,
        const std::string& p_message,
        const char* p_file = nullptr,
        const int p_line = 0,
        const char* p_func = nullptr,
        const std::chrono::system_clock::time_point& p_timestamp = std::chrono::system_clock::now(),
        const std::thread::id& p_threadId = std::this_thread::get_id()
    ) override {
        std::lock_guard<std::mutex> lk(m_mutex);
        RotationFileSink_st::write(
            p_level,
            p_message,
            p_file,
            p_line,
            p_func,
            p_timestamp,
            p_threadId
        );
    }

public:
    RotationFileSink_mt(
        const std::string& p_filename,
        std::shared_ptr<FZXLog::FMT::BaseFormatter> p_formatter,
        const Level& p_minLevel = Level::Trace,
        const size_t p_size = 1048576,
        const int p_count = 10
    ) :
        RotationFileSink_st(
            p_filename,
            p_formatter,
            p_minLevel,
            p_size,
            p_count
        )
    {}

    void flush() override {
        std::lock_guard<std::mutex> lk(m_mutex);
        RotationFileSink_st::flush();
    }
};

} // namespace FZXLog::Sink