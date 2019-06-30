#ifndef CIDK_STR_HPP
#define CIDK_STR_HPP

#include <string>
#include <sstream>

namespace cidk {
  using namespace std;

  inline void append(ostream &s) { }

  template <typename A0, typename...Args>
  void append(ostream &s, const A0 &a0, const Args &...args) {
    s << a0;
    append(s, args...);
  }

  template <typename...Args>
  string str(const Args &...args) {
    stringstream out;
    append(out, args...);
    return out.str();
  }
}

#endif