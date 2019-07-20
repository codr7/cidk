#include "cidk/opts.hpp"

namespace cidk {
  Opts::Opts(): env_escape(false), env_extend(false) {}

  void Opts::merge(const Opts &src) {
    env_escape |= src.env_escape;
    env_extend |= src.env_extend;
  }
}

