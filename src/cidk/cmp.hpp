#ifndef CIDK_CMP_HPP
#define CIDK_CMP_HPP

#include <functional>

namespace cidk {
  using namespace std;

  struct Pos;
  
  template <typename T>
  using Cmp = function<int (const Pos &pos, const T&, const T&)>;
  
  template <typename T>
  int cmp(const Pos &pos, const T &x, const T &y) {
    if (x < y) { return -1; }
    return (x > y) ? 1 : 0;
  }
}

#endif
