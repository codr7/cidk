#include "cidk/pos.hpp"

namespace cidk {
  const Pos Pos::MISSING("n/a", -1, -1);
  
  Pos::Pos(string src, int row, int col): src(src), row(row), col(col) { }
}
