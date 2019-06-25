#ifndef CIDK_REF_HPP
#define CIDK_REF_HPP

#include <list>

namespace cidk {
  using namespace std;

  struct Cx;
  struct Pos;
  
  struct Ref {
    enum struct State { _, keep, sweep };
      
    Cx &cx;
    list<Ref *>::iterator it;
    State state;

    Ref(Cx &cx);
    virtual ~Ref();
    
    virtual void sweep(const Pos &pos) = 0;
  };
}

#endif
