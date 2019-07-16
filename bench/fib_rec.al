defun fib1(n Int)(Int) {
  if {lt n 2;} n {
    dec n;
    call fib1;
    dec n;
    call fib1; 
    add;
  };
};

clock 10 {push 20; call fib1; drop;};
dump;

defun fib2(_ A)(A) {
  if {dup; lt $ 2;} _ {
    dec; dup;
    call fib2; 
    swap; dec;
    call fib2; 
    add;
  };
};

clock 10 {push 20; call fib2; drop;};
dump;
