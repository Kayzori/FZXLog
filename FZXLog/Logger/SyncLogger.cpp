#include "SyncLogger.h"

#include <algorithm>
#include <chrono>
#include <sstream>

namespace FZXLog::Logger {

SyncLogger::~SyncLogger() {
    flush();
}

void SyncLogger::addSink(std::shared_ptr<FZXLog::Sink::BaseSink> p_sink) {
    std::lock_guard<std::mutex> lk(m_mutex);
    m_sinks.emplace(std::move(p_sink));
}

void SyncLogger::removeSink(std::shared_ptr<FZXLog::Sink::BaseSink> p_sink) {
    std::lock_guard<std::mutex> lk(m_mutex);
    m_sinks.erase(std::move(p_sink));
}

void SyncLogger::clearSinks() {
    std::lock_guard<std::mutex> lk(m_mutex);
    m_sinks.clear();
}

void SyncLogger::log(const SourceLocation& p_loc, const Level& p_level, const std::string& p_message) {
    if (static_cast<int>(p_level) < static_cast<int>(m_level) && p_level == Level::Off)
        return;

    std::unordered_set<std::shared_ptr<FZXLog::Sink::BaseSink>> sinksCopy;
    {
        std::lock_guard<std::mutex> lk(m_mutex);
        sinksCopy = m_sinks;
    }

    const std::chrono::time_point timestamp = std::chrono::system_clock::now();
    const std::thread::id threadId = std::this_thread::get_id();

    for (auto& sink : sinksCopy) {
        if (sink) sink->log(p_loc, p_level, p_message, timestamp, threadId);
    }

    if (static_cast<int>(p_level) >= static_cast<int>(m_flushLevel))
        flush();
}

void SyncLogger::flush() {
    std::unordered_set<std::shared_ptr<FZXLog::Sink::BaseSink>> sinksCopy;
    {
        std::lock_guard<std::mutex> lk(m_mutex);
        sinksCopy = m_sinks;
    }

    for (auto& sink : sinksCopy) {
        if (sink) sink->flush();
    }
}

} // namespace FZXLog::Logger