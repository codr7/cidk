#ifndef CIDK_ARG_HPP
#define CIDK_ARG_HPP

#include <string>
#include <vector>

#include "cidk/stack.hpp"

namespace cidk {
  using namespace std;

  struct Cx;
  struct Env;
  struct List;
  struct Pos;
  struct Sym;
  struct Type;
  struct Val;
  
  struct Arg {
    string id_name;
    const Sym *id;
    Type *type;
    
    Arg(const Sym *id, Type *type);
    Arg(string id, Type *type = nullptr);
  };

  struct ArgList {
    vector<Arg> items;
    void parse(Cx &cx, const Pos &pos, const List &in, Env &env, Stack &stack);
  };
  
  struct Ret {
    Type &type;
    Ret(Type &type);
  };
}

#endif
