#ifndef CIDK_REPL_HPP
#define CIDK_REPL_HPP

#include <iostream>

namespace cidk {
  using namespace std;
  
  struct Cx;
  
  void repl(Cx &cx, istream &in, ostream &out);
}

#endif
