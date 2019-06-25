#ifndef CIDK_REF_HPP
#define CIDK_REF_HPP

#include <functional>
#include <list>

namespace cidk {
  using namespace std;

  struct Cx;
  struct Pos;
  
  struct Ref {
    using SweepImp = function<void (const Pos &)>;

    Cx &cx;
    list<Ref *>::iterator it;
    SweepImp sweep;
    int marks;

    Ref(Cx &cx, SweepImp sweep);
    ~Ref();
  };
}

#endif
