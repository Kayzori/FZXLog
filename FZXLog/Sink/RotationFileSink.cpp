#include "RotationFileSink.h"

#include <sstream>
#include <filesystem>

using namespace FZXLog::Sink;

RotationFileSink_st::RotationFileSink_st(
    const std::string& p_filename,
    std::shared_ptr<FZXLog::Fmt::BaseFormatter> p_formatter,
    const Level& p_minLevel,
    const Level& p_flushLevel,
    const size_t p_size,
    const int p_count,
    const size_t p_flushThreshold
) :
    BaseSink(p_formatter, p_minLevel),
    m_filename(p_filename),
    m_size(p_size),
    m_currentSize(0),
    m_count(p_count),
    m_buffer(),
    m_flushThreshold(p_flushThreshold)
{
    open();
}

RotationFileSink_st::~RotationFileSink_st() {
    try { flushBuffer(); } catch (...) {}
    if (m_ofs.is_open()) {
        try {
            m_ofs << "[END LOG]\n";
            m_ofs.flush();
        } catch (...) {}
        m_ofs.close();
    }
}

void RotationFileSink_st::open() {
    if (m_ofs.is_open()) m_ofs.close();
    // ensure directory exists
    try {
        std::filesystem::path p(m_filename);
        if (p.has_parent_path()) std::filesystem::create_directories(p.parent_path());

        // open in append mode
        m_ofs.open(m_filename, std::ios::out | std::ios::app);
        if (m_ofs.is_open()) {
            if (std::filesystem::exists(m_filename)) {
                m_currentSize = static_cast<size_t>(std::filesystem::file_size(m_filename));
            } else {
                m_currentSize = 0;
            }
            // Begin marker for already written files
            m_ofs << "[BEGIN LOG]\n";
            m_ofs.flush();
        }
    } catch (...) {
        // best-effort: leave m_ofs closed on failure
    }
}

void RotationFileSink_st::rotate() {
    // Flush buffer before rotating
    try { flushBuffer(); } catch (...) {}

    if (m_ofs.is_open()) {
        // write end marker so rotated files contain explicit end of log
        const std::string endMarker = "[END LOG]\n";
        try {
            m_ofs << endMarker;
            m_ofs.flush();
            m_currentSize += endMarker.size();
        } catch (...) {}
        m_ofs.close();
    }

    try {
        // shift files: name.(count-1) -> name.count, ... name -> name.1
        for (int i = m_count - 1; i >= 1; --i) {
            std::filesystem::path oldp = m_filename + std::string(".") + std::to_string(i);
            std::filesystem::path newp = m_filename + std::string(".") + std::to_string(i + 1);
            if (std::filesystem::exists(oldp)) {
                if (std::filesystem::exists(newp)) std::filesystem::remove(newp);
                std::filesystem::rename(oldp, newp);
            }
        }

        // rotate current file to .1
        std::filesystem::path p1 = m_filename + std::string(".1");
        if (std::filesystem::exists(p1)) std::filesystem::remove(p1);
        if (std::filesystem::exists(m_filename)) std::filesystem::rename(m_filename, p1);

    } catch (...) {
        // swallow exceptions; rotation is best-effort
    }

    // reopen new file and reset counters
    open();
    m_currentSize = 0;
}

void RotationFileSink_st::write(
    const SourceLocation& p_loc,
    const Level& p_level,
    const std::string& p_message,
    const std::chrono::system_clock::time_point& p_timestamp,
    const std::thread::id& p_threadId
) {
    std::string out = "";
    if (m_formatter) {
        out = m_formatter->format(
            p_level,
            p_message,
            p_loc.file,
            p_loc.line,
            p_loc.func,
            p_timestamp,
            p_threadId
        );
    }

    if (out.empty() || out.back() != '\n') out.push_back('\n');

    const size_t lineSize = out.size();

    // If single line larger than max size, flush buffer and write directly
    if (lineSize >= m_size) {
        flushBuffer();
        if (!m_ofs.is_open()) open();
        if (m_ofs.is_open()) {
            m_ofs << out;
            m_ofs.flush();
            m_currentSize = static_cast<size_t>(std::filesystem::file_size(m_filename));
            if (m_currentSize >= m_size) rotate();
        }
        return;
    }

    // If adding line to current on-disk size (including buffer) would exceed max, flush buffer then rotate if needed
    if (m_currentSize + m_buffer.size() + lineSize >= m_size) {
        flushBuffer();
        if (m_currentSize + lineSize >= m_size) {
            rotate();
        }
    }

    // Append to buffer
    m_buffer += out;

    // Flush buffer if threshold reached
    if (m_buffer.size() >= m_flushThreshold) {
        flushBuffer();
    }
}

void RotationFileSink_st::flush() {
    try { flushBuffer(); } catch (...) {}
    if (m_ofs.is_open()) m_ofs.flush();
}

void RotationFileSink_st::flushBuffer() {
    if (m_buffer.size() <= 0) return;
    if (!m_ofs.is_open()) open();
    if (!m_ofs.is_open()) return;

    m_ofs << m_buffer;
    m_ofs.flush();

    m_currentSize += m_buffer.size();

    // clear buffer
    m_buffer.clear();

    if (m_currentSize >= m_size) rotate();
}
