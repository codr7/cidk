#include <sstream>

#include "cidk/e.hpp"
#include "cidk/str.hpp"
#include "cidk/sym.hpp"
#include "cidk/type.hpp"

namespace cidk {
  using namespace std;
  
  void E::print(ostream &out) const {
    out << "Error";
    if (pos.src) { out << " in " << *pos.src; }
    out << " at row " << pos.row << ", col " << pos.col << ":\n" << msg;
  }

  const char* E::what() const throw() {
    if (!is_cached) {
      stringstream buf;
      print(buf);
      auto *t(const_cast<E *>(this));
      t->msg = buf.str();
      t->is_cached = true;
    }

    return msg.c_str();
  }
}
