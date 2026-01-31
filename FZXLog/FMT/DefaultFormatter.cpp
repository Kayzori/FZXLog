#include "DefaultFormatter.h"

#include <sstream>
#include <iomanip>

std::string FZXLog::FMT::DefaultFormatter::format(
    const Level& p_level,
    const std::string& p_message,
    const char* p_file,
    const int p_line,
    const char* p_func,
    const std::chrono::system_clock::time_point& p_timestamp,
    const std::thread::id& p_threadId,
    const bool p_ANSIColor
) {
    std::ostringstream ss;

    // Timestamp
    const bool hasColor = ((m_flags & FZXLOG_FMT_COLOR) && p_ANSIColor);
    const bool hasDate = (m_flags & FZXLOG_FMT_DATE);
    const bool hasTime = (m_flags & FZXLOG_FMT_TIME);
    const bool hasMs   = (m_flags & FZXLOG_FMT_MS);

    if (hasDate || hasTime) {
        const auto tt = std::chrono::system_clock::to_time_t(p_timestamp);
        std::tm tm{};
#if defined(_MSC_VER)
        localtime_s(&tm, &tt);
#else
        localtime_r(&tt, &tm);
#endif
        ss << '[';
        bool printed = false;
        if (hasDate) {
            ss << std::put_time(&tm, "%Y-%m-%d");
            printed = true;
        }
        if (hasTime) {
            if (printed) ss << ' ';
            ss << std::put_time(&tm, "%H:%M:%S");
            if (hasMs) {
                const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(p_timestamp.time_since_epoch()) % 1000;
                ss << '.' << std::setw(3) << std::setfill('0') << ms.count();
            }
        }
        ss << ']';
    }

    // Level
    if (m_flags & FZXLOG_FMT_LEVEL) {
        ss << ' ';
        if (hasColor) {
            ss << getANSIColor(p_level) << '[' << levelToString(p_level) << ']';
        } else {
            ss << '[' << levelToString(p_level) << ']';
        }
    }

    // Thread
    if (m_flags & FZXLOG_FMT_THREAD) {
        ss << ' ' << '[' << "thread:" << p_threadId << ']';
    }

    // File:line
    if ((m_flags & FZXLOG_FMT_FILELINE) && p_file && p_line > 0) {
        ss << ' ' << p_file << ':' << p_line;
    }

    // Func
    if (m_flags & FZXLOG_FMT_FUNC) {
        ss << ' ';
        if (p_func && *p_func) ss << p_func << " - ";
        else ss << "- ";
    } else {
        // separator between metadata and message
        ss << ' ';
    }

    // Message
    ss << p_message;

    if (hasColor) {
        ss << FZXLOG_ANSI_RESET;
    }


    return ss.str();
}
