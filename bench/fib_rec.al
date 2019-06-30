fun fib(Int)(Int) {
  dup;
  push 2;
  call <;
  
  if _ {
    call dec;
    dup;

    call fib;
    swap;
    
    call dec;
    call fib;
    call +;
  };
};

bench 10 10 {
  push 20;
  call fib;
};

dump _;