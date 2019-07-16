defun fib1((n a b) Int)(Int) {
  if {is n 0;} a {
    if {is n 1;} b {
      dec n; 
      push b; 
      add a b; 
      call fib1;
    };
  };
};

clock 10000 {push 20 0 1; call fib1; drop;};
dump;

defun fib2((n a b) A)(A) {
  if n {
    if {is n 1;} b {
      dec n; 
      push b; 
      add a b; 
      recall;
    };
  } a;
};

clock 10000 {push 20 0 1; call fib2; drop;};
dump;
