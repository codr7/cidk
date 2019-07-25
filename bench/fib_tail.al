defconst REPS 10000 N 20;

defun fib1((n a b) Int)(Int) {
  gt n 1; if $ {
    dec n; 
    push b; 
    add a b; 
    call fib1;
  } {
    is n 0; if $ a b;
  };
};

clock REPS {push N 0 1; call fib1; drop;};
dump;

defun fib2((n a b) Int)(Int) {
  gt n 1; if $ {
    dec n; 
    push b; 
    add a b; 
    recall;
  } {   
    is n 0; if $ a b;
  };
};

clock REPS {push N 0 1; call fib2; drop;};
dump;

defun fib3((_ _ _) Int)(Int) {
  swap 2; cp;
 
  gt $ 1; if $ {
    dec;
    swap 2; cp; swap 2;
    add;
    recall;
  } {
    cp; is 0; if $ {
      swap 1 2; drop 2; 
    } {
      drop 2;       
    };
  };
};

push N 0 1; call fib3; dump;

clock REPS {push N 0 1; call fib3; drop;};
dump;
