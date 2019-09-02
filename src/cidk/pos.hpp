#ifndef CIDK_POS_HPP
#define CIDK_POS_HPP

#include <optional>

#include "cidk/path.hpp"

namespace cidk {
  using namespace std;
  
  struct Pos {
    static const Pos _;
    
    optional<Path> src;
    int row, col;
    
    Pos(const optional<Path> &src = {}, int row = 1, int col = 0);
  };
}

#endif
