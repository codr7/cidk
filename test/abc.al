push 35 7;
add;
assert(stack) {is 42;};

push 7 21;
add $ 14 $;
assert(stack) {is 42;};

sub 49 7;
assert(stack) {is 42;};

mul 6 7;
assert(stack) {is 42;};

push (1 2 3) (1 2 3);
assert(stack) {eq;};

lt 7 14;
assert(stack) {is T;};

gt 7 14;
assert(stack) {is F;};

assert() {eq "foo\"bar" "foo\"bar";};

push 1 2 3; cp;
assert(stack) {eq stack (1 2 3 3);};

push 1 2 3 4 5;
cp 4 3;
assert(stack) {eq stack (1 2 3 4 5 2 3 4);};

push 1 2 3;
cp T T;
assert(stack) {eq stack (1 2 3 1 2 3);};

push 42 7;
poke 35 _;
add;
assert (stack) {is 42;};

push 1 2 3;
poke {mul 2;} _;
assert (stack) {eq stack (1 4 3);};

do-env _ {
  defconst foo 42;
  assert(foo) {is foo 42;};
};

do-env _ {
  defconst foo 42;
  do-env T {assert(foo) {is foo 42;};};
};

do-stack (7 14 21) {
  add; add;
  assert(stack) {is 42;};
};

if T 42 T;
assert(stack) {is 42;};

if F 42 7;
assert(stack) {is 7;};

do-env _ {
  defun foo(_ Int)(Int) { cp; add; };
  push 21;
  call foo;
  assert(stack) {is 42;};
};

do-env _ {
  defun foo(_ Int)() { dec; cp; if $ {recall;} _; };
  push 42;
  call foo;
  assert(stack) {is 0;};
};
