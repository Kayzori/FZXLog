#include "ConsoleSink.h"

#include <iostream>

void FZXLog::Sink::ConsoleSink_st::write(
    const Level& p_level,
    const std::string& p_message,
    const char* p_file,
    const int p_line,
    const char* p_func,
    const std::chrono::system_clock::time_point& p_timestamp,
    const std::thread::id& p_threadId
) {
    std::string out;
    if (m_formatter) {
        out = m_formatter->format(
            p_level,
            p_message,
            p_file,
            p_line,
            p_func,
            p_timestamp,
            p_threadId,
            true
        );
    } else out = p_message;
    std::cout << out << '\n';
}

void FZXLog::Sink::ConsoleSink_st::flush() {
    std::cout.flush();
}
