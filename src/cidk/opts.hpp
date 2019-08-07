#ifndef CIDK_OPTS_HPP
#define CIDK_OPTS_HPP

#include <map>
#include <vector>

namespace cidk {
  using namespace std;

  struct ExtId;
  struct Pos;
  struct Sym;
  
  struct Opts {
    map<const Sym *, size_t> regs;
    vector<ExtId> ext_ids;
    
    size_t push_ext_id(const Sym *id);
    size_t push_reg(const Pos &pos, const Sym *id);
    optional<size_t> push_reg(const Sym *id);
  };
};

#endif

