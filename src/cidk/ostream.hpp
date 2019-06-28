#ifndef CIDK_OSTREAM_HPP
#define CIDK_OSTREAM_HPP

#include <ostream>

#include "cidk/ref.hpp"

namespace cidk {
  struct OStream: Ref {
    ostream &imp;
    
    OStream (Cx &cx, ostream &imp);
    virtual void sweep(const Pos &pos) override;
  };
}

#endif
