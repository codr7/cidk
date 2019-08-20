#include <cassert>

#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ext_id.hpp"
#include "cidk/expr.hpp"
#include "cidk/fun.hpp"
#include "cidk/types/list.hpp"

namespace cidk {
  Fun::Fun(Cx &cx,
           const Pos &pos,
           Env &env,
           const Sym *id,
           const vector<Arg> &args,
           const vector<Ret> &rets,
           Fimp imp): Def(cx, pos, id), env(cx, env), imp(imp) {
    copy(args.begin(), args.end(), back_inserter(this->args.items));
  }
  
  void Fun::init(Cx &cx, const Pos &pos, Env &env) {
    Val *root(env.try_get(pos, id));

    if (root) {
      if (root->type != &cx.list_type) { throw ESys(pos, "Dup binding: ", id); }
    } else {
      List *l(cx.list_type.pool.get(cx));
      root = &env.let(cx, pos, id, Val(cx.list_type, l));
    }

    auto &rl(*root->as_list);
    Val rk(cx.fun_type, this);
    auto i(bsearch(pos, rl.items, rk));
    assert(i == rl.items.end());
    rl.items.insert(i, rk);
    
    stringstream buf;
    buf << id << '[';
    char sep(0);
    
    for (auto &a: args.items) {
      if (!a.id) { a.id = cx.intern(pos, a.id_name); }
      if (!a.type) { a.type = &cx.a_type; }
      weight += a.type->tag;
      if (sep) { buf << sep; }
      buf << a.type->id;
      sep = ' ';
    }

    buf << ']';
    id = cx.intern(pos, buf.str());
  }

  void Fun::mark() {
    if (!ref_mark) {
      ref_mark = true;
      env.mark_refs();
      mark_refs(body);
      body_opts.mark_refs();
    }
  }  

  bool Fun::match(Val *stackp, size_t stack_len) const {
    auto &as(args.items);
    
    if (auto nas(as.size()); nas) {
      if (nas <= stack_len) {
        for (auto a = &as.back(); a >= &as[0]; a--, stackp--) {          
          if (Type *at(a->type); at != &cx.a_type && !stackp->type->isa(*at)) {
            return false;
          }
        }

        return true;
      }

      return false;
    }

    return true;
  }
  
  void Fun::sweep(Cx &cx, const Pos &pos) {
    Val &root(env.get(pos, id->root));
    auto &rl(*root.as_list);
    auto i(bsearch(pos, rl.items, Val(cx.fun_type, this)));
    assert(i->as_fun == this);
    rl.items.erase(i);
    cx.fun_type.pool.put(this);
  }
}
