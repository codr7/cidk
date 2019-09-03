#include <fstream>

#include "cidk/call.hpp"
#include "cidk/cidk.hpp"
#include "cidk/cx.hpp"
#include "cidk/defer.hpp"
#include "cidk/e.hpp"
#include "cidk/ext_id.hpp"
#include "cidk/op.hpp"
#include "cidk/ops/stash.hpp"
#include "cidk/read.hpp"
#include "cidk/str.hpp"
#include "cidk/types/bool.hpp"
#include "cidk/types/expr.hpp"
#include "cidk/types/fix.hpp"
#include "cidk/types/int.hpp"
#include "cidk/types/nil.hpp"
#include "cidk/types/list.hpp"
#include "cidk/types/pop.hpp"
#include "cidk/types/reg.hpp"
#include "cidk/types/str.hpp"
#include "cidk/val.hpp"

namespace cidk {
  Cx::Cx():
    env(*this),
    meta_type(env.add_type<MetaType>(*this, Pos::_, "Meta")),
    nil_type(env.add_type<NilType>(*this, Pos::_, "Nil")),
    a_type(env.add_type<Type>(*this, Pos::_, "A")),
    num_type(env.add_type<Type>(*this, Pos::_, "Num", {&a_type})),
    bool_type(env.add_type<BoolType>(*this, Pos::_, "Bool", {&a_type})),
    char_type(env.add_type<CharType>(*this, Pos::_, "Char", {&a_type})),
    expr_type(env.add_type<ExprType>(*this, Pos::_, "Expr", {&a_type})),
    fix_type(env.add_type<FixType>(*this, Pos::_, "Fix", {&num_type})),
    fun_type(env.add_type<FunType>(*this, Pos::_, "Fun", {&a_type})),
    int_type(env.add_type<IntType>(*this, Pos::_, "Int", {&num_type})),
    lib_type(env.add_type<LibType>(*this, Pos::_, "Lib", {&a_type})),
    list_type(env.add_type<ListType>(*this, Pos::_, "List", {&a_type})),
    funs_type(env.add_type<FunsType>(*this, Pos::_, "Funs", {&list_type})),
    ostream_type(env.add_type<OStreamType>(*this, Pos::_, "OStream", {&a_type})),
    pop_type(env.add_type<PopType>(*this, Pos::_, "Pop", {&a_type})),
    reg_type(env.add_type<RegType>(*this, Pos::_, "Reg", {&a_type})),
    str_type(env.add_type<StrType>(*this, Pos::_, "Str", {&a_type})),
    sym_type(env.add_type<SymType>(*this, Pos::_, "Sym", {&a_type})),
    eval_state(EvalState::go),
    regp(&regs[0]),
    stackp(&stack[0]),
    deferp(&defers[0]),
    call(nullptr),
    _(nil_type),
    $(pop_type),
    T(bool_type, true),
    F(bool_type, false),
    eop(sym_type, intern(Pos::_, ";")),
    stdin(&cin), stdout(&cout), stderr(&cerr) {
    init_op_types(*this);
  }

  void Cx::deinit() {
    clear_refs();
    env.clear(*this);
    sweep_refs(Pos::_);
    for (auto &s: syms) { sym_pool.put(s.second); }
  }

  void Cx::add_op_type(const OpType &t) { op_types.emplace(t.id, &t); }

  void Cx::clear_refs() {
    for (auto i(refs.next); i != &refs; i = i->next) { i->get().ref_mark = false; }
  }
  
  void Cx::eval(Ops &in, Env &env, Reg *regs) {
    ops.push_back(&in);   
    auto min_defer(deferp);

    try {
      for (Op &o: in) {
        o.eval(*this, env, regs); 
        if (eval_state != EvalState::go) { break; }
      }
    } catch (exception &e) {
      eval_defers(min_defer, env, regs);
      ops.pop_back();
      regp = regs;

      throw;
    }

    eval_defers(min_defer, env, regs);
    ops.pop_back();
    regp = regs;
  }

  const Sym *Cx::intern(const Pos &pos, const string &name) {
    auto ok(syms.find(name));
    if (ok != syms.end()) { return ok->second; }    
    auto s(sym_pool.get(name));
    syms.emplace(make_pair(name, s));
    return s;
  }
  
  void Cx::load(const Pos &pos,
                const Path &src,
                Read read,
                Env &env,
                Ops &out,
                Opts &opts) {
    auto fp(src.is_absolute() ? src : load_path/src);
    ifstream f(fp);
    if (f.fail()) { throw ESys(pos, "File not found: ", fp); }

    Pos p(intern(pos, src));
    auto prev(load_path);
    load_path = src.parent_path();
    Ops ops;
    read(*this, p, f, ops);
    compile(ops, opts, env);
    move(ops.begin(), ops.end(), back_inserter(out));
    load_path = prev;
  }
  
  void Cx::mark_refs() {
    clear_refs();
    env.mark_refs();
    for (Val *v(&stack[0]); v < stackp; v++) { v->mark_refs(); }
    for (Val *v(&regs[0]); v < regp; v++) { if (v->type) { v->mark_refs(); } }
    for (Ops *os: ops) { cidk::mark_refs(*os); }
    for (Call *c(call); c; c = c->prev) { c->fun.mark(); }
    for (auto d(&defers[0]); d < deferp; d++) { d->second.mark_refs(); }
  }

  void Cx::dump_stack(ostream &out) const {
    out << '(';
    char sep(0);
      
    for (const Val *v = &stack[0]; v != stackp; v++) {
      if (sep) { out << sep; }
      v->dump(out);
      sep = ' ';
    }
      
    out << ')';
  }

  void Cx::sweep_refs(const Pos &pos) {
    for (auto i(refs.prev); i != &refs;) {
      Ref &r(i->get());
      i = i->prev;
      
      if (!r.ref_mark) {
        r.unlink();
        r.sweep(*this, pos);
      }
    }
  }
}
