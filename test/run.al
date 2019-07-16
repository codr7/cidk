push 35 7;
add;
assert(stack) {is 42;};

push 7 21;
add $ 14 $;
assert(stack) {is 42;};

do-env _ {
  define foo 42;
  assert(foo) {is foo 42;};
};

do-env _ {
  define foo 42;
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
  defun foo(_ Int)(Int) { dup; add; };
  push 21;
  call foo;
  assert(stack) {is 42;};
};

do-env _ {
  defun foo(_ Int)() { dec; dup; if $ {recall;} _; };
  push 42;
  call foo;
  assert(stack) {is 0;};
};

mark; sweep;
