#ifndef CIDK_OP_HPP
#define CIDK_OP_HPP

#include <any>
#include <unordered_map>
#include <string>

#include "cidk/ops.hpp"
#include "cidk/opts.hpp"
#include "cidk/pos.hpp"
#include "cidk/stack.hpp"
#include "cidk/sym.hpp"

namespace cidk {
  using namespace std;

  struct Cx;
  struct Env;
  struct Op;
  struct Pos;
  
  struct OpType {    
    string id;
    OpType(const string &id);

    virtual void compile(Cx &cx,
                         OpIter &in,
                         const OpIter &end,
                         Env &env,
                         Stack &stack,
                         Ops &out,
                         Opts *opts) const;

    virtual void eval(Op &op, Env &env, Stack &stack) const;
    virtual void get_ids(const Op &op, IdSet &out) const;
    virtual void mark_refs(Op &op) const;
    virtual void read(Cx &cx, Pos &pos, istream &in, Ops &out) const = 0;
  };

  struct Op {
    static unordered_map<string, OpType *> &types();

    Pos pos;
    const OpType *type;
    any data;
    
    template <typename T, typename...Args>
    Op(Cx &cx, const Pos &pos, const T &type, Args &&...args): pos(pos), type(&type) {
      type.init(cx, *this, forward<Args>(args)...);
    }

    template <typename T>
    T &as() { return any_cast<T &>(data); }

    template <typename T>
    const T &as() const { return any_cast<const T &>(data); }

    void compile(Cx &cx,
                 OpIter &in,
                 const OpIter &end,
                 Env &env, 
                 Stack &stack, 
                 Ops &out, 
                 Opts *opts);

    void eval(Env &env, Stack &stack);
    void get_ids(IdSet &out) const;
    void mark_refs();
  };
}

#endif
