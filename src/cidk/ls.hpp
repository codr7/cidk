#ifndef CIDK_LS_HPP
#define CIDK_LS_HPP

namespace cidk {  
  template <typename T, typename TagT = T>
  struct Ls {
    Ls<T, TagT> *prev, *next;

    Ls(): prev(this), next(this) {}

    void append(Ls<T, TagT> &ls) {
      ls.prev = this;
      ls.next = next;
      next->prev = &ls;
      next = &ls;
    }

    void push(Ls<T, TagT> &ls) { prev->append(ls); }
  
    void unlink() {
      prev->next = next;
      next->prev = prev;
    }

    T &val() { return static_cast<T &>(*this); }
  };
}

#endif
