#pragma once

#include "Sink.h"

#include <mutex>
#include <fstream>

#define FZXLOG_ROTATION_FILE_NAME_FMT "%s.%zu" // base_filename.index

namespace FZXLog::Sink {

class RotationFileSink_st : public Sink {
private:

    // Private members

    std::string m_base_filename;
    size_t m_max_file_size;
    size_t m_current_file_index;
    std::ofstream m_current_file;

    void open_current_file() noexcept;
    void rotate_file() noexcept;

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

    RotationFileSink_st(
        const std::string& p_base_filename,
        std::shared_ptr<FZXLog::Fmt::Formatter> p_formatter,
        const Level& p_level = Level::Trace,
        const Level& p_flush_level = Level::Error,
        size_t p_max_file_size = 10 * 1024 * 1024 // 10 MB
    ) noexcept;
    virtual ~RotationFileSink_st() override;

    // Methods

    virtual void flush() noexcept override;
};

class RotationFileSink_mt : public RotationFileSink_st {
private:

    // Mutex for thread safety

    mutable std::mutex m_mutex;

protected:

    // Protected methods

    virtual void write(
        const SourceLocation& p_loc,
        const Level& p_level,
        const std::string& p_message,
        const std::chrono::system_clock::time_point& p_timestamp = std::chrono::system_clock::now(),
        const std::thread::id& p_threadId = std::this_thread::get_id()
    ) noexcept override {
        std::lock_guard<std::mutex> lock(m_mutex);
        RotationFileSink_st::write(p_loc, p_level, p_message, p_timestamp, p_threadId);
    }

public:

    // Constructor/Destructor

    RotationFileSink_mt(
        const std::string& p_base_filename,
        std::shared_ptr<FZXLog::Fmt::Formatter> p_formatter,
        const Level& p_level = Level::Trace,
        const Level& p_flush_level = Level::Error,
        size_t p_max_file_size = 10 * 1024 * 1024 // 10 MB
    ) noexcept :
        RotationFileSink_st(
            p_base_filename,
            std::move(p_formatter),
            p_level,
            p_flush_level,
            p_max_file_size
        )
    {}
    virtual ~RotationFileSink_mt() override = default;

    // Public methods

    virtual void flush() noexcept override {
        std::lock_guard<std::mutex> lock(m_mutex);
        RotationFileSink_st::flush();
    }
};

} // namespace FZXLog::Sink
