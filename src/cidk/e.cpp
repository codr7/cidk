#include <sstream>

#include "cidk/e.hpp"
#include "cidk/str.hpp"
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

  ERead::ERead(const Pos &pos, const string &msg): E(pos, msg) { }

  EEval::EEval(const Pos &pos, const string &msg): E(pos, msg) { }

  ESys::ESys(const Pos &pos, const string &msg): E(pos, msg) { }

  EDupVar::EDupVar(const Pos &pos, const Sym *id): E(pos), id(id) { }

  void EDupVar::print(ostream &out) const {
    E::print(out);
    out << "Dup var: " << id;
  }

  ENotImplemented::ENotImplemented(const Pos &pos): ESys(pos, "Not implemented") { }

  ENotSupported::ENotSupported(const Pos &pos, const string &msg):
    ESys(pos, str("Not supported: ", msg)) { }

  EUnknownId::EUnknownId(const Pos &pos, const Sym *id):
    ESys(pos, "Unknown id: "), id(id) { }

  void EUnknownId::print(ostream &out) const {
    ESys::print(out);
    out << id;
  }

  EUnknownOp::EUnknownOp(const Pos &pos, const string &id):
    ESys(pos, "Unknown op: "), id(id) { }

  void EUnknownOp::print(ostream &out) const {
    ESys::print(out);
    out << id;
  }

  EWrongType::EWrongType(const Pos &pos, const string &msg, Type *type):
    ESys(pos, msg), type(type) { }

  void EWrongType::print(ostream &out) const {
    ESys::print(out);
    out << type->id;
  }
}
