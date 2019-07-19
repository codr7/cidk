#include "cidk/cx.hpp"
#include "cidk/str.hpp"
#include "cidk/types/str.hpp"

namespace cidk {
  Str::Str(Cx &cx, const string &bytes):
    Ref(cx), data(cx.str_type.conv.from_bytes(bytes)) {}

  Str::Str(Cx &cx, wstring chars): Ref(cx), data(move(chars)) {}

  void Str::sweep(Cx &cx, const Pos &pos) { cx.str_type.pool.put(this); }

  string Str::to_utf8(Cx &cx) const { return cx.str_type.conv.to_bytes(data); }

  string &replace(string &s, const string &ss1, const string &ss2) {
    auto i(s.find(ss1));
 
    while(i != string::npos) {
      s.replace(i, ss1.size(), ss2);
      i = s.find(ss1, i + ss2.size());
    }

    return s;
  }
}
