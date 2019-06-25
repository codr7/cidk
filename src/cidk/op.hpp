#ifndef CIDK_OP_HPP
#define CIDK_OP_HPP

#include <any>
#include <string>

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
    const OpType *type;
    any data;
    
    template <typename T, typename...Args>
    Op(const T &type, Args &&...args);

    template <typename T>
    T as() const;

    void eval(Cx &cx, const Pos &pos) const;
  };

  template <typename T, typename...Args>
  void init(Op &op, const T &type, Args &&...args);

  template <typename T, typename...Args>
  Op::Op(const T &type, Args &&...args): type(&type) {
    init(*this, type, forward<Args>(args)...);
  }

  template <typename T>
  T Op::as() const {
    return any_cast<T>(data);
  }
}

#endif
