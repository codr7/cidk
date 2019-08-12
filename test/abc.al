push 35 7; call +[Int Int];
assert({cp;}) {is 42;};

push 49 7; call -[Int Int];
assert({cp;}) {is 42;};

push 7 6; call *[Int Int];
assert({cp;}) {is 42;};

push 42 6; call /[Int Int];
assert({cp;}) {is 7;};

push (1 2 3) (1 2 3);
assert({cp 1;}) {eq;};

push 7 14; call <[Int Int];
assert({cp;}) {is T;};

push 7 14; call >[Int Int];
assert({cp;}) {is F;};

assert() {eq "foo\"bar" "foo\"bar";};

do-env {
  defconst foo 42;
  assert(foo) {is foo 42;};
};

do-env {
  defconst foo 42;
  do-env {assert(foo) {is foo 42;};};
};

if T 42 T;
assert({cp;}) {is 42;};

if F 42 7;
assert({cp;}) {is 7;};

do-env {
  defun foo(_ Int)(Int) { cp; call +[Int Int]; };
  push 21;
  call foo;
  assert({cp;}) {is 42;};
};

do-env {
  defun foo(_ Int)() { dec; cp; if $ {recall;} _; };
  push 42;
  call foo;
  assert({cp;}) {is 0;};
};
