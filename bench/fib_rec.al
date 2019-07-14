defun fib(_ Int)(Int) {
  if {dup; lt $ 2;} _ {
    dec; dup;
    call fib; 
    swap; dec;
    call fib; 
    add;
  };
};

clock 10 {push 20; call fib; drop; mark; sweep;};
dump;
