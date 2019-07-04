#ifndef CIDK_TIMER_HPP
#define CIDK_TIMER_HPP

#include <chrono>
#include <cstdint>

namespace cidk {
  using namespace std;
  
  struct Timer {
    using Clock = chrono::steady_clock;
    Clock::time_point start;

    Timer();
    uint64_t ms() const;
  };
}

#endif
