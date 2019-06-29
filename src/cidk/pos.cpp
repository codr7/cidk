#include "cidk/pos.hpp"

namespace cidk {
  const Pos Pos::_("n/a", -1, -1);
  
  Pos::Pos(const string &src, int row, int col): src(src), row(row), col(col) { }
}
