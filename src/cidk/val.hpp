#ifndef CIDK_VAL_HPP
#define CIDK_VAL_HPP

#include "cidk/cmp.hpp"
#include "cidk/e.hpp"
#include "cidk/fix.hpp"
#include "cidk/opts.hpp"
#include "cidk/pos.hpp"
#include "cidk/types/char.hpp"
#include "cidk/types/int.hpp"

namespace cidk {
  using namespace std;

  struct EUser;
  struct Expr;
  struct Fun;
  struct Lib;
  struct List;
  struct OStream;
  struct Pair;
  struct Str;

  struct V {
    union {
      bool       as_bool;
      Char       as_char;
      Env       *as_env;
      Expr      *as_expr;
      EUser     *as_e;
      Fix        as_fix;
      Fun       *as_fun;
      Int        as_int;
      Lib       *as_lib;
      List      *as_list;
      OStream   *as_ostream;
      Pair      *as_pair;
      void      *as_ptr;
      size_t     as_reg;
      Str       *as_str;
      const Sym *as_sym;
      Type      *as_type;
    };
  };
  
  struct Val: V {
    ValType *type = nullptr;
    const Sym *id = nullptr;
    
    Val() {}
  
    Val(ValType &type): type(&type) {}

    template <typename ValT>
    Val(TValType<ValT> &type, ValT val): type(&type) { type.set(*this, val); }

    void clear() {
      id = nullptr;
      type = nullptr;
    }
    
    Val &clone(const Pos &pos, Val &dst) const {
      dst = *this;
      type->clone(pos, dst, *this);
      return dst;
    }

    int cmp(const Pos &pos, const Val &y) const {
      return (y.type == type)
        ? type->cmp(pos, *this, y)
        : cidk::cmp(pos, type, y.type);
    }
    
    void compile(const Pos &pos, Env &env, Opts &opts) {
      type->compile(pos, *this, env, opts);
    }

    void dump(ostream &out) const {
      if (type) {
        type->dump(*this, out);
      } else {
        out << "n/a";
      }
    }

    bool eq(const Pos &pos, const Val &y) const { return type->eq(pos, *this, y); }

    bool eval(const Pos &pos, Env &env, Reg *regs) const {
      return type->eval(pos, *this, env, regs);
    }

    bool find_op(function<bool (Ops &, OpIter &)> pred) const {
      return type->find_op(*this, pred);
    }
    
    bool is(const Val &y) const {
      if (type != y.type) { return false; }
      return type->is(*this, y);
    }

    bool is_eop() const;

    void mark_refs() {
      type->mark();
      type->mark_refs(*this);
    }

    template <typename ValT>
    void reset(TValType<ValT> &type, ValT val) {
      this->type = &type;
      type.set(*this, val);
    }

    void splat(const Pos &pos, int max) const { return type->splat(pos, *this, max); }
    
    void sweep(const Pos &pos) { return type->sweep(pos, *this); }

    bool get_bool() const { return type->get_bool(*this); }
  };

  inline ostream &operator <<(ostream &out, const Val &v) {
    v.dump(out);
    return out;
  }

  template <>
  inline int cmp(const Pos &pos, const Val &x, const Val &y) { return x.cmp(pos, y); }
}

#endif
