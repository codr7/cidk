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
