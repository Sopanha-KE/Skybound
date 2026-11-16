#include "sky/core/time.hpp"

#include <chrono>

namespace sky {
namespace {

class SystemClock final : public IClock {
public:
    Nanoseconds now() const override {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(
                   std::chrono::steady_clock::now().time_since_epoch())
            .count();
    }
};

}  // namespace

std::unique_ptr<IClock> makeSystemClock() { return std::make_unique<SystemClock>(); }

}  // namespace sky
