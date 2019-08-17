defconst REPS 10 N 20;

defun fib1(n Int)(Int) {
  push n 2; 
  call <[Any Any];
  
  if $ n {
    step n -1;
    call fib1[Int];
    step n -1;
    call fib1[Int];
    call +[Int Int];
  };
};

clock REPS {push N; call fib1[Int]; drop;};
dump;

defun fib2(_ Int)(Int) {
  cp; 
  push 2; 
  call <[Any Any]; 
  
  if $ _ {
    step $ -1; 
    cp;
    call fib2[Int];
    swap; 
    step $ -1;
    call fib2[Int];
    call +[Int Int]; 
  };
};

clock REPS {push N; call fib2[Int]; drop;};
dump;
