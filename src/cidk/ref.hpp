#ifndef CIDK_REF_HPP
#define CIDK_REF_HPP

#include <list>

namespace cidk {
  using namespace std;

  struct Cx;
  struct Pos;

  enum struct RefState { _, mark, sweep };
  
  struct Ref {    
    Cx &cx;
    list<Ref *>::iterator it;
    RefState ref_state;

    Ref(Cx &cx);
    virtual ~Ref();
    
    virtual void sweep(const Pos &pos) = 0;
  };
}

#endif
