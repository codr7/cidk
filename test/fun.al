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
  defun foo((i j) Int)(Int) {
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
