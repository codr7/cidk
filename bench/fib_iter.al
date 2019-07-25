defconst REPS 10000 N 20;

defun fib1(n Int)(Int) {
  let a 0 b 1;
  
  dec n; for $ F {
    swap a b;
    set b {add a;}; 
  };

  push b;
};

clock REPS {push N; call fib1; drop;};
dump;

defun fib2(_ Int)(Int) {
  push 1 0;
  swap 2;
   
  dec; for $ F {
    cp; swap 2;
    add;
  };

  swap; drop;
};

clock REPS {push N; call fib2; drop;};
dump;
