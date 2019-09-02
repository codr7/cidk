#ifndef CIDK_E_HPP
#define CIDK_E_HPP

#include <exception>

#include "cidk/pos.hpp"
#include "cidk/str.hpp"

namespace cidk {
  using namespace std;

  struct E: exception {
    Pos pos;
    string msg;
    bool is_cached;

    template <typename...Args>
    E(const Pos &pos, Args &&...args):
      pos(pos), msg(str(forward<Args>(args)...)), is_cached(false) {}
    
    virtual const char* what() const throw() override;
    virtual void print(ostream &out) const;
  };

  struct ESys: E {
    template <typename...Args>
    ESys(const Pos &pos, Args &&...args): E(pos, forward<Args>(args)...) {}
  };

  struct EUser: E {
    template <typename...Args>
    EUser(const Pos &pos, Args &&...args): E(pos, forward<Args>(args)...) {}
  };
}

#endif
