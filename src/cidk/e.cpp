#include <sstream>

#include "cidk/e.hpp"
#include "cidk/type.hpp"

namespace cidk {
  using namespace std;
  
  E::E(const Pos &pos, const string &msg): pos(pos), msg(msg) { }

  void E::print(ostream &out) const {
    out <<
      "Error in " << pos.src <<
      " at row " << pos.row << ", col " << pos.col << ":\n" <<
      msg;
  }

  const char* E::what() const throw() {
    stringstream buf;
    print(buf);
    return buf.str().c_str();
  }

  ReadE::ReadE(const Pos &pos, const string &msg): E(pos, msg) { }

  RunE::RunE(const Pos &pos, const string &msg): E(pos, msg) { }

  SysE::SysE(const Pos &pos, const string &msg): E(pos, msg) { }

  NotImplemented::NotImplemented(const Pos &pos): SysE(pos) { }

  void NotImplemented::print(ostream &out) const {
    E::print(out);
    out << "Not implemented";
  }

  UnknownId::UnknownId(const Pos &pos, const Sym *id): E(pos), id(id) { }

  void UnknownId::print(ostream &out) const {
    E::print(out);
    out << "Unknown id: " << id;
  }
}
