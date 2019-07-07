#ifndef CIDK_ARG_HPP
#define CIDK_ARG_HPP

#include <string>

namespace cidk {
  using namespace std;

  struct Sym;
  struct Type;
  
  struct Arg {
    string id_name;
    const Sym *id;
    Type *type;
    
    Arg(const Sym *id, Type *type);
    Arg(string id, Type *type = nullptr);
  };

  struct Ret {
    Type &type;
    Ret(Type &type);
  };
}

#endif
