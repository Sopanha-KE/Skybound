#include "sky/core/log.hpp"

#include <catch2/catch_test_macros.hpp>

#include <string>
#include <utility>
#include <vector>

TEST_CASE("LogScope routes formatted messages to the sink", "[log]") {
    std::vector<std::pair<sky::LogLevel, std::string>> seen;
    {
        sky::LogScope scope(sky::LogLevel::Info, [&](sky::LogLevel l, std::string_view m) {
            seen.emplace_back(l, std::string(m));
        });
        sky::logInfo("value {}", 5);
        sky::logWarn("careful");
    }
    REQUIRE(seen.size() == 2);
    REQUIRE(seen[0].first == sky::LogLevel::Info);
    REQUIRE(seen[0].second == "value 5");
    REQUIRE(seen[1].first == sky::LogLevel::Warn);
}

TEST_CASE("LogScope filters below the minimum level", "[log]") {
    int count = 0;
    sky::LogScope scope(sky::LogLevel::Warn, [&](sky::LogLevel, std::string_view) { ++count; });
    sky::logDebug("hidden");
    sky::logInfo("hidden");
    sky::logError("shown");
    REQUIRE(count == 1);
}

TEST_CASE("Nested LogScope restores the outer logger", "[log]") {
    int outer = 0, inner = 0;
    sky::LogScope a(sky::LogLevel::Info, [&](sky::LogLevel, std::string_view) { ++outer; });
    {
        sky::LogScope b(sky::LogLevel::Info, [&](sky::LogLevel, std::string_view) { ++inner; });
        sky::logInfo("to inner");
    }
    sky::logInfo("to outer");
    REQUIRE(inner == 1);
    REQUIRE(outer == 1);
}
