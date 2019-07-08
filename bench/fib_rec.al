defun fib(Int)(Int) {
  if {dup; push 2; call <;} _ {
    call dec; dup;
    call fib; swap;
    call dec fib +;
  };
};

clock 10 {push 20; call fib; drop;};
dump;
