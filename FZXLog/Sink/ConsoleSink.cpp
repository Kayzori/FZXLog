#include "ConsoleSink.h"

#include <thread>
#include <chrono>
#include <iostream>

void FZXLog::Sink::ConsoleSink_st::write(
    const SourceLocation& p_loc,
    const Level& p_level,
    const std::string& p_message,
    const std::chrono::system_clock::time_point& p_timestamp,
    const std::thread::id& p_threadId
) {
    std::string out = ""; // Init as Empty

    if (color) out += getANSICode(p_level);

    // Format
    if (m_formatter) {
        out += m_formatter->format(
            p_level,
            p_message,
            p_loc.file,
            p_loc.line < 0 ? -1 : p_loc.line,
            p_loc.func,
            p_timestamp,
            p_threadId
        );
    }

    if (color) out += FZXLOG_ANSICODE_RESET;

    // Print Consol
    std::cout << out << '\n';
}

void FZXLog::Sink::ConsoleSink_st::flush() {
    std::cout.flush();
}
