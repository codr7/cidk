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
    int indent;
    Env env;
    
    Reader(Cx &cx, const Pos &pos, istream &in);

    void read_ops(Ops &out);
    void read_op(Ops &out);
    
    optional<Val> read_val();
    optional<Val> read_id();
    optional<Val> read_list();
    optional<Val> read_num();

    int read_indent();
  };
}

#endif
