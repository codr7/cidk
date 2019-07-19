#include "cidk/cx.hpp"
#include "cidk/types/str.hpp"
#include "cidk/val.hpp"

namespace cidk {
  StrType::StrType(Cx &cx,
                     const Pos &pos,
                     const Sym *id,
                     const vector<Type *> &parents):
    TValType<Str *>(cx, pos, id, parents) {}

  void StrType::clone(const Pos &pos, Val &dst, const Val &src) const {
    dst.as_str = cx.str_type.pool.get(cx, src.as_str->data);
  }

  void StrType::cp(Val &dst, const Val &src) const { dst.as_str = src.as_str; }

  void StrType::dump(const Val &val, ostream &out) const {
    auto s(val.as_str->to_utf8(val.type->cx));

    for (auto i(s.begin()); i != s.end(); i++) {
      if (*i == '"') {
        s.insert(i, '\\');
        i++;
      }
    }
    
    out << '"' << s << '"';
  }

  bool StrType::eq(const Pos &pos, const Val &x, const Val &y) const {
    return x.as_str->data == y.as_str->data;
  }

  bool StrType::is(const Val &x, const Val &y) const {
    return x.as_str == y.as_str;
  }

  void StrType::mark_refs(const Val &val) const { val.as_str->ref_mark = true; }

  void StrType::set(const Pos &pos, Val &dst, Str *val) const { dst.as_str = val; }


  void StrType::splat(const Pos &pos, const Val &val, Env &env, Stack &stack) const {
    for (auto c: val.as_str->data) { stack.emplace_back(pos, cx.char_type, c); }
  }

  void StrType::sweep(const Pos &pos, Val &val) { val.as_str->sweep(cx, pos); }

  bool StrType::get_bool(const Val &val) const { return !val.as_str->data.empty(); }
}
