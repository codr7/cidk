#ifndef CIDK_READER_HPP
#define CIDK_READER_HPP

#include "cidk/env.hpp"
#include "cidk/pos.hpp"
#include "cidk/ops.hpp"

namespace cidk {
  struct Cx;
  struct Val;
  
  struct Reader {
    Cx &cx;
    Pos pos;
    istream &in;
    
    Reader(Cx &cx, const Pos &pos, istream &in);

    void read_ops(Env &env, Ops &out);
    bool read_op(Env &env, Ops &out);
    void read_eop(Env &env);
    
    optional<Val> read_val(Env &env);
    Val read_expr(Env &env);
    Val read_id(Env &env);
    Val read_list(Env &env);
    Val read_num(Env &env);

    void skip_ws();
  };
}

#endif
