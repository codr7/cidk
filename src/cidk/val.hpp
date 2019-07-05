#ifndef CIDK_VAL_HPP
#define CIDK_VAL_HPP

#include "cidk/pos.hpp"
#include "cidk/types/int.hpp"

namespace cidk {
  using namespace std;

  struct Expr;
  struct Fun;
  struct List;
  struct Macro;
  struct OStream;
  struct ValType;
  
  template <typename ValT>
  struct TValType;
  
  struct Val {
    ValType *type;
    
    union {
      bool as_bool;
      Env *as_env;
      Expr *as_expr;
      Fun *as_fun;
      Int as_int;
      List *as_list;
      Macro *as_macro;
      OStream *as_ostream;
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
    
    void call(const Pos &pos, Env &env, Stack &stack);
    Val &clone(const Pos &pos, Val &dst) const;
    void dump(const Pos &pos, ostream &out) const;
    Val &dup(Val &dst) const;
    bool eq(const Pos &pos, const Val &y) const;
    void eval(const Pos &pos, Env &env, Stack &stack) const;
    void get_ids(IdSet &out) const;
    bool is(const Pos &pos, const Val &y) const;
    bool is_eop() const;
    void mark_refs(const Pos &pos);

    template <typename ValT>
    void reset(const Pos &pos, TValType<ValT> &type, ValT val) {
      this->type = &type;
      type.set(pos, *this, val);
    }

    void splat(const Pos &pos, Env &env, Stack &stack);
    void sweep(const Pos &pos);

    bool Bool(const Pos &pos) const;
  };
}

#endif
