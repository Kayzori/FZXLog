#pragma once

#include "BaseSink.h"

namespace FZXLog::Sink {

class ConsoleSink_st : public BaseSink {
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
    ConsoleSink_st(
        std::shared_ptr<FZXLog::FMT::BaseFormatter> p_formatter,
        const Level& p_minLevel = Level::Trace
    ) :
        BaseSink(p_formatter, p_minLevel)
    {}

    virtual ~ConsoleSink_st() = default;

    void flush() override;
};

class ConsoleSink_mt : public ConsoleSink_st {
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
        ConsoleSink_st::write(
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
    ConsoleSink_mt(
        std::shared_ptr<FZXLog::FMT::BaseFormatter> p_formatter,
        const Level& p_minLevel = Level::Trace
    ) :
        ConsoleSink_st(p_formatter, p_minLevel)
    {}

    void flush() override {
        std::lock_guard<std::mutex> lk(m_mutex);
        ConsoleSink_st::flush();
    }
};


} // namespace FZXLog::Sink