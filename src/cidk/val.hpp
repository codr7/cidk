#ifndef CIDK_VAL_HPP
#define CIDK_VAL_HPP

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
  
  template <typename ValT>
  struct TValType;
  
  struct Val {
    ValType *type;
    
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
        
    Val();
    Val(const Val &src);
    Val(ValType &type);

    template <typename ValT>
    Val(TValType<ValT> &type, ValT val): type(&type) { type.set(*this, val); }
    
    ~Val();
    const Val &operator =(const Val &src);
    
    Val &clone(const Pos &pos, Val &dst) const;
    Val &cp(Val &dst) const;
    void compile(Cx &cx, const Pos &pos, Env &env, Stack &stack, Opts &opts);
    void dump(ostream &out) const;
    bool eq(const Pos &pos, const Val &y) const;
    void eval(Cx &cx, const Pos &pos, Env &env, Reg *regs, Stack &stack) const;
    void get_ids(IdSet &out) const;
    bool is(const Val &y) const;
    bool is_eop() const;
    void mark_refs();

    void push(Cx &cx, const Pos &pos, Env &env, Reg *regs, Stack &stack) const;

    template <typename ValT>
    void reset(TValType<ValT> &type, ValT val) {
      this->type = &type;
      type.set(*this, val);
    }

    void splat(const Pos &pos, Env &env, Stack &stack) const;
    void sweep(const Pos &pos);

    bool get_bool() const;
    Env &get_env();
  };

  ostream &operator <<(ostream &out, const Val &v);
}

#endif
