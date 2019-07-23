defconst REPS 10000 N 20;

defun fib1(n Int)(Int) {
  let a 0 b 1;
  
  for {dec n;} {  
    swap a b;
    set b {add a b; cp;}; 
  };
};

clock REPS {push N; call fib1; drop;};
dump;
