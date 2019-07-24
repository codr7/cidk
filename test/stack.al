push 1 2 3; cp;
assert(stack) {eq stack (1 2 3 3);};

push 1 2 3 4 5;
cp 3 3;
assert(stack) {eq stack (1 2 3 4 5 2 3 4);};

push 1 2 3;
cp T T;
assert(stack) {eq stack (1 2 3 1 2 3);};

push 1 2 3;
swap 0 1;
assert(stack) {eq stack (1 3 2);};

push 1 2 3;
swap 1;
assert(stack) {eq stack (1 3 2);};

do-env T {
  let foo 1 bar 2;
  swap foo bar;
  push foo bar;
  assert(stack) {eq stack (2 1);};
};

do-env T {
  let foo 1;
  push 2;
  swap foo;
  push foo;
  assert(stack) {eq stack (1 2);};
};

push 1 2 3;
set 1 4;
assert(stack) {eq stack (1 4 3);};

do-env T {
  let foo 1;
  set foo 2;
  assert(stack) {eq foo 2;};
};

do-env T {
  let foo 1;
  set foo {add 41;};
  assert(foo) {eq foo 42;}; 
};

push 42 7;
poke 35 _;
add;
assert (stack) {is 42;};

push 1 2 3;
poke {mul 2;} _;
assert (stack) {eq stack (1 4 3);};

do-stack (7 14 21) {
  add; add;
  assert(stack) {is 42;};
};
