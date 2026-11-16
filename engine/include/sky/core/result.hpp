#pragma once

#include <cassert>
#include <optional>
#include <string>
#include <utility>
#include <variant>

namespace sky {

struct Error {
    std::string message;
};

// C++20 has no std::expected; this is the minimal subset we need. Accessing the wrong
// alternative is a programming error (asserts), not a recoverable condition.
template <class T>
class Result {
public:
    Result(T value) : data_(std::move(value)) {}
    Result(Error error) : data_(std::move(error)) {}

    bool ok() const { return std::holds_alternative<T>(data_); }
    explicit operator bool() const { return ok(); }

    T& value() & {
        assert(ok());
        return std::get<T>(data_);
    }
    const T& value() const& {
        assert(ok());
        return std::get<T>(data_);
    }
    T&& value() && {
        assert(ok());
        return std::get<T>(std::move(data_));
    }
    const Error& error() const {
        assert(!ok());
        return std::get<Error>(data_);
    }

private:
    std::variant<T, Error> data_;
};

template <>
class Result<void> {
public:
    Result() = default;
    Result(Error error) : error_(std::move(error)) {}

    bool ok() const { return !error_.has_value(); }
    explicit operator bool() const { return ok(); }
    const Error& error() const {
        assert(!ok());
        return *error_;
    }

private:
    std::optional<Error> error_;
};

}  // namespace sky
