#ifndef CIDK_READ_HPP
#define CIDK_READ_HPP

#include "cidk/ops.hpp"

namespace cidk {
  struct Env;
  struct Pos;
  struct Val;
  
  void read_ops(Pos &pos, istream &in, Env &env, Ops &out);
  bool read_op(Pos &pos, istream &in, Env &env, Ops &out);
  void read_eop(Pos &pos, istream &in, Env &env);
  
  optional<Val> read_val(Pos &pos, istream &in, Env &env);
  Val read_expr(Pos &pos, istream &in, Env &env);
  Val read_id(Pos &pos, istream &in, Env &env);
  Val read_list(Pos &pos, istream &in, Env &env);
  Val read_num(Pos &pos, istream &in, Env &env);

  void skip_ws(Pos &pos, istream &in);
}

#endif
