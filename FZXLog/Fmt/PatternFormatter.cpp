#include "PatternFormatter.h"

#include <sstream>
#include <iomanip>

namespace FZXLog::Fmt {

std::string PatternFormatter::format(
    const SourceLocation& p_location,
    const FZXLog::Level& p_level,
    const std::string& p_message,
    const std::chrono::system_clock::time_point& p_timestamp,
    const std::thread::id& p_thread_id
) const noexcept {
    if (p_level == Level::Off) return "";

    const std::time_t tt = std::chrono::system_clock::to_time_t(p_timestamp);
    std::tm tm{};
    localtime_s(&tm, &tt);

    const auto ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            p_timestamp.time_since_epoch()
        ) % 1000;

    std::ostringstream out;
    out.fill('0');

    for (size_t i = 0; i < m_pattern.size(); ++i) {
        char c = m_pattern[i];
        if (c != '%') {
            out << c;
            continue;
        }

        if (++i >= m_pattern.size())
            break;

        switch (m_pattern[i]) {
            case '%': out << '%'; break;

            // date
            case 'y':
                out << std::setw(4) << (tm.tm_year + 1900);
                break;
            case 'm':
                out << std::setw(2) << (tm.tm_mon + 1);
                break;
            case 'd':
                out << std::setw(2) << tm.tm_mday;
                break;

            // time
            case 'H':
                out << std::setw(2) << tm.tm_hour;
                break;
            case 'M':
                out << std::setw(2) << tm.tm_min;
                break;
            case 'S':
                out << std::setw(2) << tm.tm_sec;
                break;
            case 'e':
                out << std::setw(3) << ms.count();
                break;

            // metadata
            case 'l':
                out << FZXLogLevelToString(p_level);
                break;

            case 't':
                out << p_thread_id;
                break;

            case 's':
                out << p_location.m_file;
                break;

            case '#':
                out << p_location.m_line;
                break;

            case '!':
                out << p_location.m_func;
                break;

            // message
            case 'v':
                out << p_message;
                break;

            default:
                out << '%' << m_pattern[i];
                break;
        }
    }

    return out.str();
}

} // namespace FZXLog::Fmt