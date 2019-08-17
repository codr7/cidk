do-env {
  defun foo()() {
    let bar 42;
    defun baz()() {push bar;};
    push baz[];
  };

  call foo[];
  call;
  assert({cp;}) {is 42;};
};

do-env {
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
  assert({cp;}) {is 42;};
};
