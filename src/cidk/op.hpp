#ifndef CIDK_OP_HPP
#define CIDK_OP_HPP

#include <any>
#include <unordered_map>
#include <string>

#include "cidk/cidk.hpp"
#include "cidk/ops.hpp"
#include "cidk/opts.hpp"
#include "cidk/pos.hpp"
#include "cidk/reg.hpp"
#include "cidk/sym.hpp"
#include "cidk/val.hpp"

namespace cidk {
  using namespace std;

  struct Cx;
  struct Env;
  struct Op;
  struct Pos;
  
  struct OpType {    
    string id;

    OpType(const string &id): id(id) {}

    virtual void compile(Cx &cx,
                         OpIter &in,
                         const OpIter &end,
                         Env &env,
                         Ops &out,
                         Opts &opts) const { out.push_back(*in); }
    
    virtual bool eval(Cx &cx, Op &op, Env &env, Reg *regs) const { return true; }

    virtual bool find_op(Op &op, function<bool (Ops &, OpIter &)> pred) const {
      return false;
    }
    
    virtual void mark_refs(Op &op) const {}
    virtual void read(Cx &cx, Pos &pos, istream &in, Ops &out) const = 0;
  };

  struct Op {
    Pos pos;
    const OpType *type;
    array<Val, CIDK_OP_ARG_MAX> args;

    template <typename T, typename...Args>
    Op(Cx &cx, const Pos &pos, const T &type, Args &&...args): pos(pos), type(&type) {
      type.init(cx, *this, forward<Args>(args)...);
    }

    void compile(Cx &cx,
                 OpIter &in,
                 const OpIter &end,
                 Env &env,
                 Ops &out,
                 Opts &opts) {
      type->compile(cx, in, end, env, out, opts);
    }
    
    bool eval(Cx &cx, Env &env, Reg *regs) {
      return type->eval(cx, *this, env, regs);
    }

    bool find_op(function<bool (Ops &, OpIter &)> pred) {
      return type->find_op(*this, pred);
    }
    
    void mark_refs() { type->mark_refs(*this); }
  };
}

#endif
