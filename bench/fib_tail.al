defconst REPS 10000;
defconst N 20;

defun fib1(n Int a Int b Int)(Int) {
  push n;
  push 1; 
  call >[Any Any]; 

  if $ {
    step n -1; 
    push b;
    push a;
    push b; 
    call +[Int Int];
    call fib1[Int Int Int];
  } {
    is n 0; 
    if $ a b;
  };
};

clock REPS {push N; push 0; push 1; call fib1[Int Int Int]; drop;};
dump;

defun fib2(n Int a Int b Int)(Int) {
  push n;
  push 1; 
  call >[Any Any]; 

  if $ {
    step n -1; 
    push b;
    push a;
    push b; 
    call +[Int Int]; 
    recall;
  } {   
    is n 0; 
    if $ a b;
  };
};

clock REPS {push N; push 0; push 1; call fib2[Int Int Int]; drop;};
dump;

defun fib3(_ Int _ Int _ Int)(Int) {
  swap 2; 
  cp;
  push 1; 
  call >[Any Any]; 
 
  if $ {
    step $ -1;
    swap 2; 
    cp; 
    swap 2;
    call +[Int Int];
    recall;
  } {
    cp; 
    is 0; 
    
    if $ {
      swap 1 2; 
      drop 2; 
    } {
      drop 2;       
    };
  };
};

clock REPS {push N; push 0; push 1; call fib3[Int Int Int]; drop;};
dump;
