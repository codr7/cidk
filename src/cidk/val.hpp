#ifndef CIDK_VAL_HPP
#define CIDK_VAL_HPP

#include "cidk/e.hpp"
#include "cidk/opts.hpp"
#include "cidk/pos.hpp"
#include "cidk/types/char.hpp"
#include "cidk/types/int.hpp"

namespace cidk {
  using namespace std;

  struct Expr;
  struct Fun;
  struct List;
  struct OStream;
  struct Str;
  struct ValType;
  
  struct Val {
    const Sym *id = nullptr;
    ValType *type = nullptr;
    
    union {
      bool       as_bool;
      Char       as_char;
      Env       *as_env;
      Expr      *as_expr;
      Fun       *as_fun;
      Int        as_int;
      List      *as_list;
      OStream   *as_ostream;
      size_t     as_reg;
      Str       *as_str;
      const Sym *as_sym;
      Type      *as_type;
    };
        
    Val() {}
  
    Val(const Val &src): type(src.type) { src.cp(*this); }

    Val(ValType &type): type(&type) {}

    template <typename ValT>
    Val(TValType<ValT> &type, ValT val): type(&type) { type.set(*this, val); }
    
    const Val &operator =(const Val &src) {
      src.cp(*this);
      return *this;
    }
    
    Val &clone(const Pos &pos, Val &dst) const {
      dst.id = id;
      dst.type = type;
      type->clone(pos, dst, *this);
      return dst;
    }

    Val &cp(Val &dst) const {
      dst.id = id;
      dst.type = type;
      if (type) { type->cp(dst, *this); }
      return dst;
    }


    void compile(const Pos &pos, Env &env, Opts &opts) {
      type->compile(pos, *this, env, opts);
    }

    void dump(ostream &out) const { type->dump(*this, out); }

    bool eq(const Pos &pos, const Val &y) const { return type->eq(pos, *this, y); }

    void eval(const Pos &pos, Env &env, Reg *regs) const {
      return type->eval(pos, *this, env, regs);
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

    void splat(const Pos &pos, Env &env) const {
      return type->splat(pos, *this, env);
    }
    
    void sweep(const Pos &pos) { return type->sweep(pos, *this); }

    bool get_bool() const { return type->get_bool(*this); }
  };

  inline ostream &operator <<(ostream &out, const Val &v) {
    v.dump(out);
    return out;
  }
}

#endif
