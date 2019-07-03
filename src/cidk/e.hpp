#ifndef CIDK_E_HPP
#define CIDK_E_HPP

#include <exception>

#include "cidk/pos.hpp"

namespace cidk {
  using namespace std;

  struct Sym;
  struct Type;
  
  struct E: exception {
    Pos pos;
    string msg;
    bool is_cached;

    E(const Pos &pos, const string &msg = "");
    virtual const char* what() const throw() override;
    virtual void print(ostream &out) const;
  };

  struct ERead: E {
    ERead(const Pos &pos, const string &msg = "");
  };

  struct EEval: E {
    EEval(const Pos &pos, const string &msg = "");
  };

  struct ESys: E {
    ESys(const Pos &pos, const string &msg = "");
  };

  struct EDupConst: E {
    const Sym *id;
    
    EDupConst(const Pos &pos, const Sym *id);
    virtual void print(ostream &out) const override;
  };

  struct EDupVar: E {
    const Sym *id;
    
    EDupVar(const Pos &pos, const Sym *id);
    virtual void print(ostream &out) const override;
  };

  struct ENotImplemented: ESys {
    ENotImplemented(const Pos &pos);
  };

  struct ENotSupported: ESys {
    ENotSupported(const Pos &pos, const string &msg);
  };

  struct EUnknownId: ESys {
    const Sym *id;
    
    EUnknownId(const Pos &pos, const Sym *id);
    virtual void print(ostream &out) const override;
  };

  struct EUnknownOp: ESys {
    string id;
    
    EUnknownOp(const Pos &pos, const string &id);
    virtual void print(ostream &out) const override;
  };

  struct EWrongType: ESys {
    Type *type;
    
    EWrongType(const Pos &pos, const string &msg, Type *type);
    virtual void print(ostream &out) const override;
  };
}

#endif
