#ifndef CIDK_LIB_ABC_HPP
#define CIDK_LIB_ABC_HPP

namespace cidk {
  struct Cx;
  struct Pos;
}

namespace cidk::libs {  
  Lib &init_abc(Cx &cx, const Pos &pos);
}

#endif
