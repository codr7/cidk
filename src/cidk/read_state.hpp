#ifndef CIDK_READ_STATE_HPP
#define CIDK_READ_STATE_HPP

namespace cidk {
  struct ReadState {
    int env_depth;
    bool env_escape, env_extend;
    
    ReadState();
  };
};

#endif

