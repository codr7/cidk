do {
  defun foo()() {
    let bar 42;
    defun baz()() {push bar;};
    push baz[];
  };

  call foo[];
  call;
  check({cp;}) {is 42;};
};

do {
  defun foo(i Int j Int)(Int) {
    if i {
      step i -1; 
      step j 1; 
      dispatch foo;
    } j;
  };

  push 42;
  push 0;
  dispatch foo;
  check({cp;}) {is 42;};
};

do {
  defconst foo (+);
  push 35;
  push 7;
  dispatch foo;
  check({cp;}) {is 42;};
};

do {
  let foo 7;
  defun bar()() {let foo 42; push foo;};
  call bar[];
  check({cp;}) {is 42;};  
};
