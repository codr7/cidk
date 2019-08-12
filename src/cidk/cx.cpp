#include <fstream>

#include "cidk/call.hpp"
#include "cidk/cidk.hpp"
#include "cidk/cx.hpp"
#include "cidk/defer.hpp"
#include "cidk/e.hpp"
#include "cidk/ext_id.hpp"
#include "cidk/libs/math.hpp"
#include "cidk/op.hpp"
#include "cidk/ops/stash.hpp"
#include "cidk/read.hpp"
#include "cidk/str.hpp"
#include "cidk/types/bool.hpp"
#include "cidk/types/expr.hpp"
#include "cidk/types/int.hpp"
#include "cidk/types/nil.hpp"
#include "cidk/types/list.hpp"
#include "cidk/types/pop.hpp"
#include "cidk/types/reg.hpp"
#include "cidk/types/str.hpp"
#include "cidk/val.hpp"

namespace cidk {
  static void bool_imp(Cx &cx, const Pos &p, const Fun &f, Env &env) {
    auto &v(*cx.stackp);
    v.reset(cx.bool_type, v.get_bool());
  }
  
  Cx::Cx():
    env(*this),
    meta_type(env.add_type<MetaType>(*this, Pos::_, "Meta")),
    a_type(env.add_type<Type>(*this, Pos::_, "A")),
    nil_type(env.add_type<NilType>(*this, Pos::_, "Nil", {&a_type})),
    any_type(env.add_type<Type>(*this, Pos::_, "Any", {&a_type})),
    num_type(env.add_type<Type>(*this, Pos::_, "Num")),
    bool_type(env.add_type<BoolType>(*this, Pos::_, "Bool", {&any_type})),
    char_type(env.add_type<CharType>(*this, Pos::_, "Char", {&any_type})),
    expr_type(env.add_type<ExprType>(*this, Pos::_, "Expr", {&any_type})),
    fun_type(env.add_type<FunType>(*this, Pos::_, "Fun", {&any_type})),
    int_type(env.add_type<IntType>(*this, Pos::_, "Int", {&any_type, &num_type})),
    list_type(env.add_type<ListType>(*this, Pos::_, "List", {&any_type})),
    ostream_type(env.add_type<OStreamType>(*this, Pos::_, "OStream", {&any_type})),
    pop_type(env.add_type<PopType>(*this, Pos::_, "Pop", {&any_type})),
    reg_type(env.add_type<RegType>(*this, Pos::_, "Reg", {&any_type})),
    str_type(env.add_type<StrType>(*this, Pos::_, "Str", {&any_type})),
    sym_type(env.add_type<SymType>(*this, Pos::_, "Sym", {&any_type})),
    eval_state(EvalState::go),
    regp(&regs[0]),
    stackp(&stack[0]),
    call(nullptr),
    _(nil_type),
    $(pop_type),
    T(bool_type, true),
    F(bool_type, false),
    eop(sym_type, intern(";")),
    stdin(&cin), stdout(&cout), stderr(&cerr) {
    libs::init_math(*this);
    env.add_const(*this, Pos::_, "_", _);
    env.add_const(*this, Pos::_, "$", $);
    env.add_const(*this, Pos::_, "T", T);
    env.add_const(*this, Pos::_, "F", F);
    
    env.add_const_expr(*this, Pos::_, "stack", {Op(*this, Pos::_, ops::Stash)});
    
    env.add_fun(*this, Pos::_, "bool", {Arg("val")}, {Ret(bool_type)}, bool_imp);
  }

  void Cx::deinit() {
    clear_refs();
    env.clear(*this);
    sweep_refs(Pos::_);

#ifndef CIDK_USE_POOL
    for (auto &s: syms) { delete s.second; }
#endif
  }

  void Cx::clear_refs() {
    for (auto i(refs.next); i != &refs; i = i->next) { i->val().ref_mark = false; }
  }

  void Cx::eval(Ops &in, Env &env, const Opts &opts, Reg *regs) {
    ops.push_back(&in);
    regp += opts.regs.size();

    auto d1(defer([&]{
          regp = regs;
          ops.pop_back();
        }));

    for (auto &src: opts.ext_ids) { regs[src.dst_reg] = src.val; }
    eval(in, env, regs);
  }

  void Cx::eval(Ops &in, Env &env, Reg *regs) {
    for (Op &o: in) { 
      o.eval(*this, env, regs); 
      if (eval_state != EvalState::go) { break; }
    }
  }
  
  const Sym *Cx::intern(const string &name) {
    auto ok(syms.find(name));
    if (ok != syms.end()) { return ok->second; }
    auto s(sym_pool.get(name));
    syms.emplace(make_pair(name, s));
    return s;
  }

  void Cx::compile(Ops &ops, Opts &opts, Env &env) {
    Ops tmp;

    for (auto i(ops.begin()); i != ops.end(); i++) {
      i->compile(*this, i, ops.end(), env, tmp, opts);
    }
    
    swap(ops, tmp);
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

    Pos p(src);
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
    for (Ops *os: ops) { cidk::mark_refs(*os); }
    for (Call *c(call); c; c = c->prev) { c->fun.mark(); }
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
      Ref &r(i->val());
      i = i->prev;
      
      if (!r.ref_mark) {
        r.unlink();
        r.sweep(*this, pos);
      }
    }
  }
}
