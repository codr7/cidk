#ifndef CIDK_E_USER_HPP
#define CIDK_E_USER_HPP

#include "cidk/e.hpp"
#include "cidk/ref.hpp"
#include "cidk/val.hpp"

namespace cidk {
  using namespace std;

  struct EUser: Ref, E {
    Val val;
    
    EUser(Cx &cx, const Pos &pos, const Val &val);
    virtual bool mark_refs() override;
    virtual void print(ostream &out) const override;
    virtual void sweep(Cx &cx, const Pos &pos) override;
  };
}

#endif
