#ifndef CIDK_READ_STATE_HPP
#define CIDK_READ_STATE_HPP

namespace cidk {
  struct ReadState {
    int env_depth;
    bool env_escape, env_extend;
    
    ReadState();

    void merge(const ReadState &src) {
      env_escape |= src.env_escape;
      env_extend |= src.env_extend;
    }
  };
};

#endif

