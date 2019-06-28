#include <iostream>
#include <sstream>

#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/reader.hpp"

namespace cidk {
  Reader::Reader(Cx &cx, const Pos &pos, istream &in): cx(cx), pos(pos), in(in) { }

  void Reader::read_op(Ops &out) {
    const Pos op(pos);
    string opc;
    in >> opc;
    pos.col += opc.size();
    
    cout << "opc: " << opc << endl;
    auto found(cx.op_types.find(opc));
    if (found == cx.op_types.end()) { throw UnknownOp(pos, opc); } 
    OpType &ot(*found->second);
    ot.read(cx, op, *this, out);
  }
  
  void Reader::read_ops(Ops &out) {
    while (!in.eof()) { read_op(out); }
  }

  optional<Val> Reader::read_val() {
    return {};
  }
}
