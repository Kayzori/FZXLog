#include "RotationFileSink.h"
#include <filesystem>

namespace FZXLog::Sink {

RotationFileSink_st::RotationFileSink_st(
    const std::string& p_base_filename,
    std::shared_ptr<FZXLog::Fmt::Formatter> p_formatter,
    const Level& p_level,
    const Level& p_flush_level,
    size_t p_max_file_size
) noexcept :
    m_base_filename(p_base_filename),
    m_max_file_size(p_max_file_size),
    m_current_file_index(0),
    Sink(std::move(p_formatter), p_level, p_flush_level)
{
    open_current_file();
}

RotationFileSink_st::~RotationFileSink_st() noexcept {
    if (m_current_file.is_open()) {
        m_current_file.flush();
        m_current_file.close();
    }
}

void RotationFileSink_st::write(
    const SourceLocation& p_loc,
    const Level& p_level,
    const std::string& p_message,
    const std::chrono::system_clock::time_point& p_timestamp,
    const std::thread::id& p_threadId
) noexcept {
    if (!m_current_file.is_open()) {
        return;
    }

    try {
        if (m_formatter) {
            m_current_file
                << m_formatter->format(
                    p_loc,
                    p_level,
                    p_message,
                    p_timestamp,
                    p_threadId
                )
                << '\n';
        }
        else {
            m_current_file << p_message << '\n';
        }
    } catch (...) {
        return;
    }

    const std::streampos pos = m_current_file.tellp();

    if (pos != static_cast<std::streampos>(-1) &&
        static_cast<size_t>(pos) >= m_max_file_size) {
        rotate_file();
    }
}

void RotationFileSink_st::flush() noexcept {
    if (m_current_file.is_open()) {
        m_current_file.flush();
    }
}

void RotationFileSink_st::rotate_file() noexcept {
    m_current_file.flush();
    m_current_file.close();

    ++m_current_file_index;

    open_current_file();
}

void RotationFileSink_st::open_current_file() noexcept {
    try {
        std::filesystem::path basePath(m_base_filename);

        // Create parent directories if they don't exist
        if (!basePath.parent_path().empty()) {
            std::filesystem::create_directories(basePath.parent_path());
        }

        std::filesystem::path filename =
            basePath.string() + "." + std::to_string(m_current_file_index);

        m_current_file.open(filename, std::ios::out | std::ios::app);
    }
    catch (...) {
        return;
    }
}

} // namespace FZXLog::Sink