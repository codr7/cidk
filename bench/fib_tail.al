defun fib(n Int a Int b Int)(Int) {
  if n {
    if {is n 1;} b {
      push n; 
      call dec; 
      push b a b; 
      add; 
      recall;
    };
  } a;
};

clock 10000 {push 20 0 1; call fib; drop;};
dump;
