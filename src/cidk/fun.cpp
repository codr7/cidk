#include <cassert>

#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ext_id.hpp"
#include "cidk/expr.hpp"
#include "cidk/fun.hpp"
#include "cidk/types/funs.hpp"
#include "cidk/types/list.hpp"

namespace cidk {
  List &Fun::get_root(Cx &cx, const Pos &pos, Env &env, const Sym *id) {
    auto found = env.try_get(pos, id);

    if (found) {
      if (found->type != &cx.funs_type) { throw ESys(pos, "Dup binding: ", id); }
      return *found->as_list;
    }

    List *l(cx.funs_type.pool.get(cx));
    return *env.let(pos, id, Val(cx.funs_type, l)).as_list;
  }

  Fun::Fun(Cx &cx,
           const Pos &pos,
           List &root,
           Env &env,
           const Sym *id,
           const vector<Arg> &args,
           const vector<Ret> &rets,
           Fimp imp): Def(cx, pos, id), root(root), env(env), imp(imp) {
    copy(args.begin(), args.end(), back_inserter(this->args.items));
  }
  
  void Fun::init(Cx &cx, const Pos &pos, Env &env) {
    Val rk(cx.fun_type, this);
    auto i(find(pos, root.items, rk));
    assert(i == root.items.end());
    root.items.insert(i, rk);
    
    stringstream buf;
    buf << id << '[';
    char sep(0);
    
    for (auto &a: args.items) {
      if (!a.id) { a.id = cx.intern(pos, a.id_name); }
      if (!a.type) { a.type = &cx.a_type.or_nil(pos); }
      weight += a.type->tag;
      if (sep) { buf << sep; }
      buf << a.type->id;
      sep = ' ';
    }

    buf << ']';
    id = cx.intern(pos, buf.str());
  }

  bool Fun::mark_refs() {
    if (Ref::mark_refs()) {
      root.mark();
      env.mark_refs();
      cidk::mark_refs(body);
      body_opts.mark_refs();
      return true;
    }

    return false;
  }  
  
  void Fun::sweep(Cx &cx, const Pos &pos) {
    auto i(find(pos, root.items, Val(cx.fun_type, this)));
    assert(i->as_fun == this);
    root.items.erase(i);
    cx.fun_type.pool.put(this);
  }
}
