#ifndef CIDK_ARG_HPP
#define CIDK_ARG_HPP

#include <string>
#include <vector>

#include "cidk/sym.hpp"

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
    const Sym *id = nullptr;
    Type *type = nullptr;
    
    Arg(const Sym *id, Type *type): id(id), type(type) {
      if (id) { id_name = id->name; }
    }
    
    Arg(string id): id_name(id) {}
    Arg(string id, Type &type): id_name(id), type(&type) {}
  };

  struct ArgList {
    vector<Arg> items;
    void parse(Cx &cx, const Pos &pos, const List &in, Env &env);
  };
  
  struct Ret {
    Type &type;
    Ret(Type &type);
  };
}

#endif
