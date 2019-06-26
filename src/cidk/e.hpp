#ifndef CIDK_E_HPP
#define CIDK_E_HPP

#include <exception>

#include "cidk/pos.hpp"

namespace cidk {
  using namespace std;

  struct Sym;
  
  struct E: exception {
    Pos pos;
    string msg;

    E(const Pos &pos, const string &msg = "");
    virtual const char* what() const throw() override;
    virtual void print(ostream &out) const;
  };

  struct ReadE: E {
    ReadE(const Pos &pos, const string &msg = "");
  };

  struct RunE: E {
    RunE(const Pos &pos, const string &msg = "");
  };

  struct SysE: E {
    SysE(const Pos &pos, const string &msg = "");
  };

  struct DupVar: E {
    const Sym *id;
    
    DupVar(const Pos &pos, const Sym *id);
    virtual void print(ostream &out) const override;
  };

  struct NotImplemented: SysE {
    NotImplemented(const Pos &pos);
    virtual void print(ostream &out) const override;
  };

  struct UnknownId: E {
    const Sym *id;
    
    UnknownId(const Pos &pos, const Sym *id);
    virtual void print(ostream &out) const override;
  };
}

#endif
