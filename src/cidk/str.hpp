#ifndef CIDK_STR_HPP
#define CIDK_STR_HPP

#include <string>
#include <sstream>

#include "cidk/ref.hpp"

namespace cidk {
  using namespace std;

  struct Str: Ref {
    wstring data;

    Str(Cx &cx, const string &bytes);
    Str(Cx &cx, wstring chars);
    virtual void sweep(Cx &cx, const Pos &pos) override;
    string to_utf8(Cx &cx) const;
  };
  
  inline void append(ostream &s) {}

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

  string &replace(string &s, const string &ss1, const string &ss2);
}

#endif
