defconst REPS 10000 N 20;

defun fib1(n Int)(Int) {
  let a 0 b 1;
  step n -1;
  
  for $ F {
    swap a b;
      
    set b {
      push a; 
      call +[Int Int];
    }; 
  };

  push b;
};

clock REPS {push N; call fib1[Int]; drop;};
dump;

defun fib2(_ Int)(Int) {
  push 1 0;
  swap 2;
  step $ -1; 

  for $ F {
    cp; 
    swap 2;
    call +[Int Int];
  };

  swap; 
  drop;
};

clock REPS {push N; call fib2[Int]; drop;};
dump;
