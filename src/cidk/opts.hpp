#ifndef CIDK_OPTS_HPP
#define CIDK_OPTS_HPP

#include <map>
#include <vector>

namespace cidk {
  using namespace std;

  struct ExtId;
  struct Op;
  struct OpType;
  struct Pos;
  struct Sym;
  
  struct Opts {
    map<const Sym *, size_t> regs;
    vector<ExtId> ext_ids;

    size_t get_reg(const Pos &pos, const Sym *id);
    void mark_refs();
    size_t push_ext_id(const Pos &pos, const Sym *id);
    size_t push_reg(const Pos &pos, const Sym *id);
    optional<size_t> try_get_reg(const Pos &pos, const Sym *id);
    optional<size_t> try_push_reg(const Pos &pos, const Sym *id);
  };
};

#endif

