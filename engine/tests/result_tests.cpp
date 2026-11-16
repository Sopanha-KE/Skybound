#include "sky/core/result.hpp"

#include <catch2/catch_test_macros.hpp>

#include <memory>

TEST_CASE("Result holds a value", "[result]") {
    sky::Result<int> r = 42;
    REQUIRE(r.ok());
    REQUIRE(r.value() == 42);
}

TEST_CASE("Result holds an error", "[result]") {
    sky::Result<int> r = sky::Error{"boom"};
    REQUIRE_FALSE(r.ok());
    REQUIRE(r.error().message == "boom");
}

TEST_CASE("Result<void> reports success and failure", "[result]") {
    REQUIRE(sky::Result<void>{}.ok());
    sky::Result<void> bad = sky::Error{"nope"};
    REQUIRE_FALSE(bad);
    REQUIRE(bad.error().message == "nope");
}

TEST_CASE("Result supports move-only values", "[result]") {
    sky::Result<std::unique_ptr<int>> r = std::make_unique<int>(7);
    REQUIRE(r.ok());
    auto p = std::move(r).value();
    REQUIRE(*p == 7);
}
