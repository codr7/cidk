#include <sstream>

#include "cidk/e.hpp"
#include "cidk/sym.hpp"
#include "cidk/type.hpp"

namespace cidk {
  using namespace std;
  
  E::E(const Pos &pos, const string &msg): pos(pos), msg(msg), is_cached(false) { }

  void E::print(ostream &out) const {
    out <<
      "Error in " << pos.src <<
      " at row " << pos.row << ", col " << pos.col << ":\n" <<
      msg;
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

  ReadE::ReadE(const Pos &pos, const string &msg): E(pos, msg) { }

  EvalE::EvalE(const Pos &pos, const string &msg): E(pos, msg) { }

  SysE::SysE(const Pos &pos, const string &msg): E(pos, msg) { }

  DupVar::DupVar(const Pos &pos, const Sym *id): E(pos), id(id) { }

  void DupVar::print(ostream &out) const {
    E::print(out);
    out << "Dup var: " << id;
  }

  NotImplemented::NotImplemented(const Pos &pos): SysE(pos) { }

  void NotImplemented::print(ostream &out) const {
    E::print(out);
    out << "Not implemented";
  }

  UnknownId::UnknownId(const Pos &pos, const Sym *id):
    SysE(pos, "Unknown id: "), id(id) { }

  void UnknownId::print(ostream &out) const {
    SysE::print(out);
    out << id;
  }

  UnknownOp::UnknownOp(const Pos &pos, const string &id):
    SysE(pos, "Unknown op: "), id(id) { }

  void UnknownOp::print(ostream &out) const {
    SysE::print(out);
    out << id;
  }

  WrongType::WrongType(const Pos &pos, const string &msg, Type *type):
    SysE(pos, msg), type(type) { }

  void WrongType::print(ostream &out) const {
    SysE::print(out);
    out << type->id;
  }
}
