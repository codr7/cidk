#ifndef CIDK_ENUM_HPP
#define CIDK_ENUM_HPP

#include <cassert>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace cidk {
  using namespace std;
  
  template <typename T>
  struct Enum {  
    struct Root {
      string id;
      uint64_t next_flag;
      vector<const Enum<T> *> alts;
      map<string, const Enum<T> *> alt_lookup;

      uint64_t get_next_flag() {
        uint64_t f(next_flag);
        next_flag <<= 1;
        return f;
      }

      Root(const string &id): id(id), next_flag(1) {}
    };

    struct Set {
      Root &root;
      uint64_t imp;

      Set(Root &root): root(root), imp(0) {}

      Set operator +(const Enum<T> &y) const { return Set(root, imp | y.flag); }

      void operator +=(const Enum<T> &y) { imp |= y.flag; }

      Set operator -(const Enum<T> &y) const { Set(root, imp & ~y.flag); }

      void operator -=(const Enum<T> &y) { imp &= ~y.flag; }
    
      bool operator ()(const Enum<T> &x) const { return imp & x.flag; }

      void dump(ostream &out) const {
        out << root.id << '(';
        char sep(0);
      
        for (const Enum<T> *a: root.alts) {
          if (imp & a->flag) {
            if (sep) { out << sep; } 
            out << a->id;
            sep = ' ';
          }
        }
      
        out << ')';
      }   
    };

    Root &root;
    string id;
    size_t index;
    uint64_t flag;
  
    Enum(Root &root, const string &id):
      root(root),
      id(id),
      index(root.alts.size()),
      flag(root.get_next_flag()) {
      assert(root.alt_lookup.emplace(id, this).second);
      root.alts.push_back(this);
    }

    void dump(ostream &out) const { out << id << ':' << root.id; }
  };

}

#endif
