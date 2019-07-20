#ifndef CIDK_OPTS_HPP
#define CIDK_OPTS_HPP

namespace cidk {
  struct Opts {
    bool env_escape, env_extend;
    
    Opts();
    void merge(const Opts &src);
  };
};

#endif

