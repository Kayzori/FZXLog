#pragma once

#include "Logger.h"
#include "FZXLog/Utils.h"
#include "FZXLog/Sink/Sink.h"

#include <unordered_set>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace FZXLog::Logger {

class SyncLogger : public Logger {
protected:
    mutable std::recursive_mutex m_mutex;

public:

    // Constructor/Destructor

    SyncLogger(
        const Level& p_level = Level::Trace,
        const Level& p_flushLevel = Level::Error,
        const size_t& p_log_trace_capacity = 100
    ) :
        Logger(p_level, p_flushLevel, p_log_trace_capacity)
    {}
    virtual ~SyncLogger() override = default;

    // Getters/Setters

    void setLevel(const Level& p_level) override {
        std::lock_guard<std::recursive_mutex> lk(m_mutex);
        m_level = p_level;
    }
    Level getLevel() const override {
        std::lock_guard<std::recursive_mutex> lk(m_mutex);
        return m_level;
    }

    void setFlushLevel(const Level& p_level) override {
        std::lock_guard<std::recursive_mutex> lk(m_mutex);
        m_flushLevel = p_level;
    }
    Level getFlushLevel() const override {
        std::lock_guard<std::recursive_mutex> lk(m_mutex);
        return m_flushLevel;
    }

    std::vector<LogRecord> getLogTrace() const override {
        std::lock_guard<std::recursive_mutex> lk(m_mutex);
        return m_log_trace;
    }

    void setLogTraceCapacity(const size_t& p_capacity) override {
        std::lock_guard<std::recursive_mutex> lk(m_mutex);
        m_log_trace_capacity = p_capacity;
        while (m_log_trace.size() > m_log_trace_capacity) {
            m_log_trace.erase(m_log_trace.begin());
        }
    }

    size_t getLogTraceCapacity() const override {
        std::lock_guard<std::recursive_mutex> lk(m_mutex);
        return m_log_trace_capacity;
    }

    // Sink management
    void addSink(std::shared_ptr<FZXLog::Sink::Sink> p_sink) override {
        std::lock_guard<std::recursive_mutex> lk(m_mutex);
        m_sinks.emplace(std::move(p_sink));
    }
    void removeSink(std::shared_ptr<FZXLog::Sink::Sink> p_sink) override {
        std::lock_guard<std::recursive_mutex> lk(m_mutex);
        m_sinks.erase(std::move(p_sink));
    }
    void clearSinks() override {
        std::lock_guard<std::recursive_mutex> lk(m_mutex);
        m_sinks.clear();
    }
    std::vector<std::shared_ptr<FZXLog::Sink::Sink>> getSinks() const override {
        std::lock_guard<std::recursive_mutex> lk(m_mutex);
        std::vector<std::shared_ptr<FZXLog::Sink::Sink>> sinks;
        sinks.reserve(m_sinks.size());
        for (const auto& sink : m_sinks) {
            sinks.push_back(sink);
        }
        return sinks;
    }

    // Raw log
    void log(const SourceLocation& p_loc, const Level& p_level, const std::string& p_message) override {
        std::lock_guard<std::recursive_mutex> lk(m_mutex);
        Logger::log(p_loc, p_level, p_message);
    }
    void log(const Level& p_level, const std::string& p_message) override { log(SourceLocation(), p_level, p_message); }

    // Flush sinks
    void flush() override {
        std::lock_guard<std::recursive_mutex> lk(m_mutex);
        std::unordered_set<std::shared_ptr<Sink::Sink>> sinksCopy = m_sinks;
        for (auto& sink : sinksCopy) {
            if (sink) sink->flush();
        }
    }

};

} // namespace FZXLog::Logger
