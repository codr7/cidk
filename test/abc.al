push 35 7;
add;
assert({cp;}) {is 42;};

push 7 21;
add $ 14 $;
assert({cp;}) {is 42;};

sub 56 7 7;
assert({cp;}) {is 42;};

mul 2 3 7;
assert({cp;}) {is 42;};

div 42 2 7;
assert({cp;}) {is 3;};

push (1 2 3) (1 2 3);
assert({cp 1;}) {eq;};

lt 7 14;
assert({cp;}) {is T;};

gt 7 14;
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
  defun foo(_ Int)(Int) { cp; add; };
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
