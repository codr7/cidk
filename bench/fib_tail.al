defconst REPS 10000 N 20;

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

clock REPS {push N 0 1; call fib1; drop;};
dump;

defun fib2((n a b) Int)(Int) {
  if n {
    if {is n 1;} b {
      dec n; 
      push b; 
      add a b; 
      recall;
    };
  } a;
};

clock REPS {push N 0 1; call fib2; drop;};
dump;

defun fib3((_ _ _) Int)(Int) {
  swap 2; cp;
  
  if $ {
    cp; is 1;
      
    if $ {
      drop; drop;
    } {
      dec;
      swap 2; cp; swap 2;
      add;
      recall;
    };
  } {
    swap 1 2; drop; drop; 
  };
};

push N 0 1; call fib3; dump;

clock REPS {push N 0 1; call fib3; drop;};
dump;
