#include "ConsoleSink.h"
#include <iostream>

namespace FZXLog::Sink {

void ConsoleSink_st::write(
    const SourceLocation& p_loc,
    const Level& p_level,
    const std::string& p_message,
    const std::chrono::system_clock::time_point& p_timestamp,
    const std::thread::id& p_threadId
) noexcept {
    if (m_formatter) {
        std::string formatted_message = m_formatter->format(p_loc, p_level, p_message, p_timestamp, p_threadId);
        if (m_colored) {
            // Add color codes based on log level
            switch (p_level) {
                case Level::Trace:
                    formatted_message = FZXLOG_ANSICODE_TRACE + formatted_message;
                    break;
                case Level::Debug:
                    formatted_message = FZXLOG_ANSICODE_DEBUG + formatted_message;
                    break;
                case Level::Info:
                    formatted_message = FZXLOG_ANSICODE_INFO + formatted_message;
                    break;
                case Level::Warning:
                    formatted_message = FZXLOG_ANSICODE_WARNING + formatted_message;
                    break;
                case Level::Error:
                    formatted_message = FZXLOG_ANSICODE_ERROR + formatted_message;
                    break;
                case Level::Fatal:
                    formatted_message = FZXLOG_ANSICODE_FATAL + formatted_message;
                    break;
                default:
                    break;
            }
        }
        formatted_message += FZXLOG_ANSICODE_RESET;
        std::cout << formatted_message << '\n';
    } else {
        std::cout << p_message << '\n';
    }
}

void ConsoleSink_st::flush() noexcept {
    std::cout.flush();
}

} // namespace FZXLog::Sink
