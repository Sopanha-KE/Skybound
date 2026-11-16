#pragma once

#include <format>
#include <functional>
#include <string>
#include <string_view>
#include <utility>

namespace sky {

enum class LogLevel { Trace, Debug, Info, Warn, Error, Critical };

using LogSink = std::function<void(LogLevel, std::string_view)>;

// Logging goes through one process-wide logger, installed by a LogScope (see ADR 0003).
// With no scope installed, messages fall back to stderr so nothing is silently dropped.
void logMessage(LogLevel level, std::string_view message);

template <class... Args>
void log(LogLevel level, std::format_string<Args...> fmt, Args&&... args) {
    logMessage(level, std::format(fmt, std::forward<Args>(args)...));
}
template <class... Args>
void logTrace(std::format_string<Args...> fmt, Args&&... args) {
    log(LogLevel::Trace, fmt, std::forward<Args>(args)...);
}
template <class... Args>
void logDebug(std::format_string<Args...> fmt, Args&&... args) {
    log(LogLevel::Debug, fmt, std::forward<Args>(args)...);
}
template <class... Args>
void logInfo(std::format_string<Args...> fmt, Args&&... args) {
    log(LogLevel::Info, fmt, std::forward<Args>(args)...);
}
template <class... Args>
void logWarn(std::format_string<Args...> fmt, Args&&... args) {
    log(LogLevel::Warn, fmt, std::forward<Args>(args)...);
}
template <class... Args>
void logError(std::format_string<Args...> fmt, Args&&... args) {
    log(LogLevel::Error, fmt, std::forward<Args>(args)...);
}

// RAII owner of the logger. Scopes nest; the destructor restores the previous logger.
// An optional extra sink receives every message that passes the level filter (used by tests).
class LogScope {
public:
    explicit LogScope(LogLevel minLevel = LogLevel::Info, LogSink extraSink = {});
    ~LogScope();
    LogScope(const LogScope&) = delete;
    LogScope& operator=(const LogScope&) = delete;

private:
    struct Impl;
    Impl* impl_;
};

}  // namespace sky
