#ifndef CIDK_OP_HPP
#define CIDK_OP_HPP

#include <any>
#include <string>

#include "cidk/pos.hpp"

namespace cidk {
  using namespace std;

  struct Cx;
  struct Op;
  struct Pos;
  
  struct OpType {
    string id;
    OpType(const string &id);
    virtual void eval(Cx &cx, const Op &op) const = 0;
  };
    
  struct Op {
    Pos pos;
    const OpType *type;
    any data;
    
    template <typename T, typename...Args>
    Op(const Pos &pos, const T &type, Args &&...args);

    template <typename T>
    T as() const;

    void eval(Cx &cx, const Pos &pos) const;
  };

  namespace ops {
    template <typename T>
    void init(Op &op, const T &type);
  }
  
  template <typename T, typename...Args>
  Op::Op(const Pos &pos, const T &type, Args &&...args): pos(pos), type(&type) {
    ops::init(*this, type, forward<Args>(args)...);
  }

  template <typename T>
  T Op::as() const {
    return any_cast<T>(data);
  }
}

#endif
