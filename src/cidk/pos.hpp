#ifndef CIDK_POS_HPP
#define CIDK_POS_HPP

#include <string>

namespace cidk {
  using namespace std;
  
  struct Pos {
    static const Pos _;
    
    string src;
    int row, col;
    
    Pos(const string &src, int row=1, int col=0);
  };
}

#endif
