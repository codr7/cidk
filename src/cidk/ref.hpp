#ifndef CIDK_REF_HPP
#define CIDK_REF_HPP

namespace cidk {
  using namespace std;

  struct Cx;
  struct Pos;

  struct Ref {    
    Cx &cx;
    bool is_marked;

    Ref(Cx &cx);
    virtual ~Ref() = default;
    
    virtual void sweep(const Pos &pos) = 0;
  };
}

#endif
