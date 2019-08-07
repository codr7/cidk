#include "cidk/e.hpp"
#include "cidk/stack.hpp"
#include "cidk/val.hpp"

namespace cidk {
  ostream &operator <<(ostream &out, const Stack &s) {
    out << '(';
    char sep(0);
    
    for (auto &v: s) {
      if (sep) { out << sep; }
      out << v;
      sep = ' ';
    }

    out << ')';
    return out;
  }
}
