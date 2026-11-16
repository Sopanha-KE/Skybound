#include "sky/core/log.hpp"

#include <spdlog/sinks/callback_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <cstdio>
#include <memory>
#include <vector>

namespace sky {
namespace {

// The single process-wide logger (ADR 0003). Only touched by LogScope and logMessage.
std::shared_ptr<spdlog::logger> g_logger;

spdlog::level::level_enum toSpdlog(LogLevel level) {
    switch (level) {
    case LogLevel::Trace: return spdlog::level::trace;
    case LogLevel::Debug: return spdlog::level::debug;
    case LogLevel::Info: return spdlog::level::info;
    case LogLevel::Warn: return spdlog::level::warn;
    case LogLevel::Error: return spdlog::level::err;
    case LogLevel::Critical: return spdlog::level::critical;
    }
    return spdlog::level::info;
}

LogLevel fromSpdlog(spdlog::level::level_enum level) {
    switch (level) {
    case spdlog::level::trace: return LogLevel::Trace;
    case spdlog::level::debug: return LogLevel::Debug;
    case spdlog::level::info: return LogLevel::Info;
    case spdlog::level::warn: return LogLevel::Warn;
    case spdlog::level::err: return LogLevel::Error;
    default: return LogLevel::Critical;
    }
}

}  // namespace

struct LogScope::Impl {
    std::shared_ptr<spdlog::logger> previous;
    std::shared_ptr<spdlog::logger> logger;
};

LogScope::LogScope(LogLevel minLevel, LogSink extraSink) : impl_(new Impl) {
    std::vector<spdlog::sink_ptr> sinks{std::make_shared<spdlog::sinks::stdout_color_sink_mt>()};
    if (extraSink) {
        auto callback = std::make_shared<spdlog::sinks::callback_sink_mt>(
            [sink = std::move(extraSink)](const spdlog::details::log_msg& msg) {
                sink(fromSpdlog(msg.level), std::string_view(msg.payload.data(), msg.payload.size()));
            });
        sinks.push_back(std::move(callback));
    }
    // Unregistered: we own it, spdlog's global registry is not used.
    impl_->logger = std::make_shared<spdlog::logger>("sky", sinks.begin(), sinks.end());
    impl_->logger->set_level(toSpdlog(minLevel));
    impl_->logger->set_pattern("[%H:%M:%S.%e] [%^%l%$] %v");
    impl_->previous = std::move(g_logger);
    g_logger = impl_->logger;
}

LogScope::~LogScope() {
    g_logger = std::move(impl_->previous);
    delete impl_;
}

void logMessage(LogLevel level, std::string_view message) {
    if (g_logger) {
        g_logger->log(toSpdlog(level), message);
    } else {
        std::fprintf(stderr, "[no-logger] %.*s\n", static_cast<int>(message.size()), message.data());
    }
}

}  // namespace sky
