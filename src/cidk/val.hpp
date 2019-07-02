#ifndef CIDK_VAL_HPP
#define CIDK_VAL_HPP

#include <any>

#include "cidk/pos.hpp"
#include "cidk/types/int.hpp"

namespace cidk {
  using namespace std;

  struct Expr;
  struct Fun;
  struct List;
  struct OStream;
  struct ValType;
  
  template <typename ValT>
  struct TValType;
  
  struct Val {
    ValType *type;
    
    union {
      any as_any;
      bool as_bool;
      Expr *as_expr;
      Fun *as_fun;
      Int as_int;
      List *as_list;
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
    
    template <typename ValT>
    ValT as() const { return any_cast<ValT>(as_any); }

    void call(const Pos &pos);
    void clone(const Pos &pos, Val &dst) const;
    void dump(const Pos &pos, ostream &out) const;
    void dup(Val &dst) const;
    bool eq(const Pos &pos, const Val &y) const;
    void eval(const Pos &pos, Env &env) const;
    bool is(const Pos &pos, const Val &y) const;
    bool is_eop() const;
    void mark_refs(const Pos &pos);

    template <typename ValT>
    void reset(const Pos &pos, TValType<ValT> &type, ValT val) {
      this->type = &type;
      type.set(pos, *this, val);
    }

    void splat(const Pos &pos);
    void sweep(const Pos &pos);

    bool Bool(const Pos &pos) const;
  };
  
  template <>
  inline Int Val::as() const {
    return as_int;
  }
}

#endif
