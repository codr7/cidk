#ifndef CIDK_POS_HPP
#define CIDK_POS_HPP

#include <optional>

namespace cidk {
  using namespace std;

  struct Sym;
  
  struct Pos {
    static const Pos _;
    
    const Sym *src;
    int row, col;
    
    Pos(const Sym *src = nullptr, int row = 1, int col = 0);
  };
}

#endif
