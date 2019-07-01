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
    Env env;
    
    Reader(Cx &cx, const Pos &pos, istream &in);

    void read_ops(Ops &out);
    bool read_op(Ops &out);
    void read_eop();
    
    optional<Val> read_val();
    Val read_expr();
    Val read_id();
    Val read_list();
    Val read_num();
  };
}

#endif
