#include "cidk/pos.hpp"

namespace cidk {
  const Pos Pos::_({}, -1, -1);
  
  Pos::Pos(const optional<Path> &src, int row, int col):
    src(src), row(row), col(col) {}
}
