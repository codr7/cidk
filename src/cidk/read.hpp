#ifndef CIDK_READ_HPP
#define CIDK_READ_HPP

#include <functional>

#include "cidk/ops.hpp"

namespace cidk {
  struct Env;
  struct Pos;
  struct Val;

  using Read = function<void (Cx &cx, Pos &pos, istream &in, Ops &out)>;
  
  void read_ops(Cx &cx, Pos &pos, istream &in, Ops &out);
  bool read_op(Cx &cx, Pos &pos, istream &in, Ops &out);
  void read_eop(Pos &pos, istream &in);
  optional<Val> read_val(Cx &cx, Pos &pos, istream &in);
  Val read_ctrl_char(Cx &cx, Pos &pos, istream &in);
  Val read_expr(Cx &cx, Pos &pos, istream &in);
  Val read_id(Cx &cx, Pos &pos, istream &in);
  Val read_list(Cx &cx, Pos &pos, istream &in);
  Val read_num(Cx &cx, Pos &pos, istream &in);
  Val read_str(Cx &cx, Pos &pos, istream &in);

  void skip_ws(Pos &pos, istream &in);
}

#endif
