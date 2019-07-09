#include "cidk/timer.hpp"

namespace cidk {
  Timer::Timer(): start(Clock::now()) {}

  uint64_t Timer::ms() const {
    return chrono::duration<double, milli>(Clock::now() - start).count();
  }
}
