#include "cidk/pos.hpp"

namespace cidk {
  const Pos Pos::_(nullptr, -1, -1);
  
  Pos::Pos(const Sym *src, int row, int col): src(src), row(row), col(col) {}
}
