#include "Logger.h"

#include <chrono>
#include <thread>

namespace FZXLog::Logger {

void Logger::log(const SourceLocation& p_loc, const Level& p_level, const std::string& p_message) {
    if (p_level == Level::Off || static_cast<uint8_t>(p_level) < static_cast<uint8_t>(m_level))
        return;

    std::unordered_set<std::shared_ptr<Sink::Sink>> sinksCopy = m_sinks;
    const auto timestamp = std::chrono::system_clock::now();
    const auto threadId = std::this_thread::get_id();

    for (auto& sink : sinksCopy) {
        if (sink) {
            sink->log(p_loc, p_level, p_message, timestamp, threadId);
        }
    }

    if (static_cast<uint8_t>(p_level) >= static_cast<uint8_t>(m_flushLevel)) {
        flush();
    }

    if (m_log_trace_capacity > 0) {
        m_log_trace.emplace_back(p_loc, p_level, p_message, timestamp, threadId);
        while (m_log_trace.size() > m_log_trace_capacity) {
            m_log_trace.erase(m_log_trace.begin());
        }
    }
}

} // namespace FZXLog::Logger
