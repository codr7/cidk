#ifndef CIDK_VAL_HPP
#define CIDK_VAL_HPP

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
      bool as_bool;
      Char as_char;
      Env *as_env;
      Expr *as_expr;
      Fun *as_fun;
      Int as_int;
      List *as_list;
      OStream *as_ostream;
      Str *as_str;
      const Sym *as_sym;
      Type *as_type;
    };
        
    Val();
    Val(const Val &src);
    Val(ValType &type);

    template <typename ValT>
    Val(const Pos &pos, TValType<ValT> &type, ValT val): type(&type) {
      type.set(pos, *this, val);
    }
    
    ~Val();

    const Val &operator =(const Val &src);
    
    Val &clone(const Pos &pos, Val &dst) const;
    Val &cp(Val &dst) const;
    void dump(ostream &out) const;
    bool eq(const Pos &pos, const Val &y) const;
    void eval(const Pos &pos, Env &env, Stack &stack) const;
    void get_ids(IdSet &out) const;
    bool is(const Val &y) const;
    bool is_eop() const;
    void mark_refs();

    template <typename ValT>
    void reset(const Pos &pos, TValType<ValT> &type, ValT val) {
      this->type = &type;
      type.set(pos, *this, val);
    }

    void splat(const Pos &pos, Env &env, Stack &stack) const;
    void sweep(const Pos &pos);

    bool get_bool() const;
    Env &get_env();
  };

  ostream &operator <<(ostream &out, const Val &v);
}

#endif
