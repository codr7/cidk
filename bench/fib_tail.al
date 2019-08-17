defconst REPS 10000 N 20;

defun fib1((n a b) Int)(Int) {
  push n 1; call >[Any Any]; if $ {
    dec n; 
    push b a b; 
    call +[Int Int] fib1[Int Int Int];
  } {
    is n 0; if $ a b;
  };
};

clock REPS {push N 0 1; call fib1[Int Int Int]; drop;};
dump;

defun fib2((n a b) Int)(Int) {
  push n 1; call >[Any Any]; if $ {
    dec n; 
    push b a b; 
    call +[Int Int]; 
    recall;
  } {   
    is n 0; if $ a b;
  };
};

clock REPS {push N 0 1; call fib2[Int Int Int]; drop;};
dump;

defun fib3((_ _ _) Int)(Int) {
  swap 2; cp;
 
  push 1; call >[Any Any]; if $ {
    dec;
    swap 2; cp; swap 2;
    call +[Int Int];
    recall;
  } {
    cp; is 0; if $ {
      swap 1 2; drop 2; 
    } {
      drop 2;       
    };
  };
};

clock REPS {push N 0 1; call fib3[Int Int Int]; drop;};
dump;
