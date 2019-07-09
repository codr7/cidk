#ifndef CIDK_OP_HPP
#define CIDK_OP_HPP

#include <any>
#include <unordered_map>
#include <string>

#include "cidk/ops.hpp"
#include "cidk/pos.hpp"
#include "cidk/stack.hpp"
#include "cidk/sym.hpp"

namespace cidk {
  using namespace std;

  struct Cx;
  struct Env;
  struct Op;
  struct Pos;
  struct ReadState;
  
  struct OpType {    
    string id;
    OpType(const string &id);
    virtual void eval(const Op &op, Env &env, Stack &stack) const = 0;
    virtual void get_ids(const Op &op, IdSet &out) const;
    
    virtual void read(Cx &cx, 
                      Pos &pos, 
                      istream &in,
                      ReadState &state,
                      Env &env,
                      Stack &stack,
                      Ops &out) const = 0;
  };

  struct Op {
    static unordered_map<string, OpType *> &types();

    Pos pos;
    const OpType *type;
    any data;
    
    template <typename T, typename...Args>
    Op(const Pos &pos, const T &type, Args &&...args): pos(pos), type(&type) {
      type.init(*this, forward<Args>(args)...);
    }

    template <typename T>
    T &as() { return any_cast<T &>(data); }

    template <typename T>
    const T &as() const { return any_cast<const T &>(data); }

    void eval(Env &env, Stack &stack) const;
    void get_ids(IdSet &out) const;
  };
}

#endif
