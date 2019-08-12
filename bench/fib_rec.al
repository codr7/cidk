defconst REPS 10 N 20;

defun fib1(n Int)(Int) {
  push n 2; call <[Int Int]; if $ n {
    dec n;
    call fib1;
    dec n;
    call fib1 +[Int Int];
  };
};

clock REPS {push N; call fib1; drop;};
dump;

defun fib2(_ Int)(Int) {
  cp; push 2; call <[Int Int]; if $ _ {
    dec; cp;
    call fib2;
    swap; dec;
    call fib2 +[Int Int]; 
  };
};

clock REPS {push N; call fib2; drop;};
dump;
