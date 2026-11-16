#pragma once

#include <cstdint>
#include <memory>

namespace sky {

using Nanoseconds = std::int64_t;

// Injected so the loop is testable and simulation code never reads wall-clock time.
class IClock {
public:
    virtual ~IClock() = default;
    virtual Nanoseconds now() const = 0;
};

std::unique_ptr<IClock> makeSystemClock();

}  // namespace sky
