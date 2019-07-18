#include "cidk/pos.hpp"

namespace cidk {
  const Pos Pos::_("n/a", -1, -1);
  
  Pos::Pos(Path src, int row, int col): src(move(src)), row(row), col(col) {}
}
