#ifndef CIDK_READER_HPP
#define CIDK_READER_HPP

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
    
    void read_ops(Ops &out);
    void read_op(Ops &out);
    optional<Val> read_val();
  };
}

#endif
