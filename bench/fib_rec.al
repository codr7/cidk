defun fib(_ Int)(Int) {
  if {dup; push 2; call <;} _ {
    call dec; dup;
    call fib; swap;
    call dec fib; add;
  };
};

clock 10 {push 20; call fib; drop; mark; sweep;};
dump;
