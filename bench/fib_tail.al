fun: fib(n Int a Int b Int)(Int) {
  if {is n 0;} a {
    if {is n 1;} b {
      push n; 
      call dec; 
      push b a b; 
      call +; 
      recall;
    };
  };
};

bench 10 {push 20 0 1; call fib; drop;};
dump;
