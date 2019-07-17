defconst REPS 10 N 20;

defun fib1(n Int)(Int) {
  if {lt n 2;} n {
    dec n;
    call fib1;
    dec n;
    call fib1; 
    add;
  };
};

clock REPS {push N; call fib1; drop;};
dump;

defun fib2(_ A)(A) {
  if {cp; lt $ 2;} _ {
    dec; cp;
    call fib2; 
    swap; dec;
    call fib2; 
    add;
  };
};

clock REPS {push N; call fib2; drop;};
dump;
