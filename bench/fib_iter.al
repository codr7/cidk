defconst REPS 10000 N 20;

defun fib1(n Int)(Int) {
  let a 0 b 1;
  
  for {dec n;} _ {  
    swap a b;
    set b {add a;}; 
  };
};

clock REPS {push N; call fib1; drop;};
dump;

defun fib2(_ Int)(Int) {
  push 1 0;
  swap 2;
  dec;
  
  for $ _ {
    drop;
    cp;
    swap 2;
    add;
  };

  swap;
  drop;
};

clock REPS {push N; call fib2; drop;};
dump;
