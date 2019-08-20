#ifndef CIDK_CMP_HPP
#define CIDK_CMP_HPP

#include <functional>

namespace cidk {
  using namespace std;

  struct Pos;
  
  template <typename X, typename Y = X>
  using Cmp = function<int (const Pos &pos, const X&, const Y&)>;
  
  template <typename X, typename Y = X>
  int cmp(const Pos &pos, const X &x, const Y &y) {
    if (x < y) { return -1; }
    return (x > y) ? 1 : 0;
  }
}

#endif
