#ifndef CIDK_CX_HPP
#define CIDK_CX_HPP

#include <iostream>
#include <string>
#include <unordered_map>

#include "cidk/env.hpp"
#include "cidk/list.hpp"
#include "cidk/op.hpp"
#include "cidk/path.hpp"
#include "cidk/pool.hpp"
#include "cidk/read.hpp"
#include "cidk/reg.hpp"
#include "cidk/sym.hpp"
#include "cidk/types/fun.hpp"
#include "cidk/types/funs.hpp"
#include "cidk/types/lib.hpp"
#include "cidk/types/meta.hpp"
#include "cidk/types/ostream.hpp"
#include "cidk/types/sym.hpp"
#include "cidk/val.hpp"

namespace cidk {
  namespace fs = experimental::filesystem;

  struct Call;
  struct CharType;
  struct BoolType;
  struct ExprType;
  struct EType;
  struct EUser;
  struct FixType;
  struct IntType;
  struct ListType;
  struct NilType;
  struct PopType;
  struct Ref;
  struct RegType;
  struct StrType;
  
  enum struct EvalState { error, go, recall };

  using Stack = Val *;

  struct Cx {
    bool debug = false;
    
    Pool<Sym> sym_pool;    
    unordered_map<string, Sym *> syms;

    unordered_map<string, const OpType *> op_types;
    Ls<Ref> refs;
    Env env;
    
    Int type_tag = 1;
    MetaType &meta_type;
    NilType &nil_type;    
    Type &a_type, &num_type;
    
    BoolType &bool_type;
    CharType &char_type;
    EType &e_type;
    ExprType &expr_type;
    FixType &fix_type;
    FunType &fun_type;
    IntType &int_type;
    LibType &lib_type;
    ListType &list_type;
    FunsType &funs_type;
    OStreamType &ostream_type;
    PopType &pop_type;
    RegType &reg_type;
    StrType &str_type;
    SymType &sym_type;
    
    vector<Ops *> ops;
    EvalState eval_state;

    array<Reg, CIDK_REG_MAX> regs;
    Reg *regp;

    array<Val, CIDK_STACK_MAX> stack;
    Val *stackp;

    using DeferItem = pair<Pos, Val>;
    array<DeferItem, CIDK_DEFER_MAX> defers;
    DeferItem *deferp;

    Call *call;
    EUser *e;
    
    const Val _, $, T, F, eop;

    Path load_path;
    istream *stdin;
    ostream *stdout, *stderr;
    
    Cx();

    void deinit();
    void add_op_type(const OpType &t);

    Reg *alloc_regs(size_t n) {
      Reg *base(regp);
      regp += n;
      fill(base, regp, Val());
      return base;
    }
    
    void clear_refs();

    void compile(Ops &ops, Opts &opts, Env &env) {
      Ops tmp;
      
      for (auto i(ops.begin()); i != ops.end(); i++) {
        i->compile(*this, i, ops.end(), env, tmp, opts);
      }
      
      swap(ops, tmp);
    }

    void dump_stack(ostream &out) const;

    void eval_defers(DeferItem *min, Env &env, Reg *regs) {
      while (deferp > min) {
        deferp--;
        deferp->second.eval(deferp->first, env, regs);
      }
    }

    void eval(Ops &in, Env &env, Reg *regs) {
      ops.push_back(&in);   
      
      for (Op &o: in) {
        o.eval(*this, env, regs); 
        if (eval_state != EvalState::go) { break; }
      }
      
      ops.pop_back();
    }

    const Sym *intern(const Pos &pos, const string &name);

    void load(const Pos &pos,
              const Path &src,
              Read read,
              Env &env,
              Ops &out,
              Opts &opts);
    
    void mark_refs();
    
    Val &peek(const Pos &pos, size_t offs = 0) {
      if (stackp - offs <= &stack[0]) { throw ESys(pos, "Stack is empty"); }
      return *(stackp - (offs + 1));
    }

    Val &pop(const Pos &pos) {
      if (stackp == &stack[0]) { throw ESys(pos, "Stack is empty"); }
      return *--stackp;
    }

    template <typename...Args>
    Val &push(const Pos &p, Args &&...args) {
      if (stackp == stack.end()) { throw ESys(p, "Stack overflow"); }
      auto &dst(*stackp);
      dst = Val(forward<Args>(args)...);
      stackp++;
      return dst;
    }

    void sweep_refs(const Pos &pos);

    optional<Val> try_pop() {
      if (stackp == &stack[0]) { return {}; }
      return *--stackp;
    }
  };

  inline void set_reg(Reg *base, size_t n, const Sym *id, const Val &src) {
    Val &dst(base[n]);
    dst = src;
    dst.id = id;
  }
  
  template <typename...Rest>
  Fun &Env::add_fun(Cx &cx,
                    const Pos &pos,
                    const string &id,
                    const vector<Arg> &args,
                    const vector<Ret> &rets,
                    Rest &&...rest) {
    return add_fun(cx, pos, cx.intern(pos, id), args, rets, forward<Rest>(rest)...);
  }

  template <typename...Rest>
  Fun &Env::add_fun(Cx &cx,
                    const Pos &pos,
                    const Sym *id,
                    const vector<Arg> &args,
                    const vector<Ret> &rets,
                    Rest &&...rest) {
    auto &ft(cx.fun_type);
    auto &root(Fun::get_root(cx, pos, *this, id));
    Fun *f(ft.pool.get(cx, pos, root, *this, id, args, rets, forward<Rest>(rest)...));
    f->init(cx, pos, *this);
    let(pos, f->id, Val(ft, f));
    return *f;
  }

  template <typename LibT, typename...Args>
  LibT &Env::add_lib(Cx &cx, const Pos &pos, const string &id, Args...args) {
    auto &lt(cx.lib_type);
    LibT *l(new LibT(cx, pos, cx.intern(pos, id), forward<Args>(args)...));
    let(pos, l->id, Val(lt, l));
    return *l;
  }

  template <typename TypeT, typename...Rest>
  TypeT &Env::add_type(Cx &cx,
                       const Pos &pos,
                       const string &id,
                       const vector<Type *> parents,
                       Rest &&...rest) {
    TypeT *t(new TypeT(cx,
                       pos,
                       *this,
                       cx.intern(pos, id),
                       parents,
                       forward<Rest>(rest)...));
    
    let(pos,
        t->id,
        Val((id == "Meta") ? *dynamic_cast<MetaType *>(t) : cx.meta_type,
            dynamic_cast<Type *>(t)));
    
    return *t;
  }

  template <typename...Rest>
  void Env::add_var(Cx &cx, const Pos &pos, const string &id, Rest &&...rest) {
    add_var(cx, pos, id, Val(forward<Rest>(rest)...));
  }

  inline typename Env::Iter Env::find(const Pos &pos, const Sym *id) {
    return cidk::find<Val, const Sym *>(pos, items, id,
                                        [&](auto &p, auto &x, auto &y) {
                                          return cmp(p, x.id, y);
                                        });
  }

  inline Val &Env::get(const Pos &pos, const Sym *id) {
    auto i(find(pos, id));
    if (i == items.end() || i->id != id) { throw ESys(pos, "Unknown id: ", id); }
    return *i;
  }

  inline typename Env::Iter Env::insert(Iter i, const Sym *id, const Val &val) {
    i = items.insert(i, val);
    i->id = id;
    return i;
  };

  inline Val &Env::let(const Pos &pos, const Sym *id, const Val &val) {
    auto i(find(pos, id));

    if (i != items.end() && i->id == id) {
      throw ESys(pos, "Duplicate binding: ", id);
    }
    
    return *insert(i, id, val);
  }

  inline void Env::set(Cx &cx,
                       const Pos &pos,
                       const Sym *id,
                       const Val &val,
                       bool force) {
    auto i(find(pos, id));
    
    if (i == items.end() || i->id != id) {
      if (!force) { throw ESys(pos, "Missing binding: ", id); }
      i = items.insert(i, val);
    } else {
      *i = val;
    }

    i->id = id;
  }

  inline Val *Env::try_get(const Pos &pos, const Sym *id) {
    auto i(find(pos, id));
    return (i == items.end() || i->id != id) ? nullptr : &*i;
  }

  inline void Env::use(Cx &cx,
                       const Pos &pos,
                       Env &src,
                       const vector<const Sym *> &ids) {
    auto use([&](const Val &src) {
        auto dst(find(pos, src.id));
        
        if (src.type == &cx.funs_type &&
            dst != items.end() &&
            dst->id == src.id &&
            dst->type == &cx.funs_type) {
          auto &sl(src.as_list->items);
          copy(sl.begin(), sl.end(), back_inserter(dst->as_list->items));
        } else if (dst == items.end() || dst->id != src.id) {
          insert(dst, src.id, src);
        } else {
          throw ESys(pos, "Dup binding: ", src.id);
        }
      });
    
    if (ids.empty()) {
      for (auto v: src.items) { use(v); }      
    } else {
      for (auto id: ids) { use(src.get(pos, id)); }
    }
  }

  inline bool Fun::match(Val *stackp, size_t min, size_t max) const {
    auto &as(args.items);
    if (as.empty()) { return true; }
    size_t n(as.size());
    if (n < min || n > max) { return false; }

    for (auto a = &as.back(); n; n--, a--, stackp--) {          
      if (Type *at(a->type); !stackp->type->isa(*at)) { return false; }
    }

    return true;
  }

  inline bool Val::is_eop() const {
    auto &cx(type->cx);
    return type == &cx.sym_type && as_sym == cx.eop.as_sym;
  }
}

#endif
