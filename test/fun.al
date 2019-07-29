do-env _ {
  defun foo()() {
    let bar 42;
    defun baz()() {push bar;};
    push baz;
  };

  call foo;
  call;
  assert(stack) {eq stack (42);};
};
