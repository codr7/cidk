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

do-stack (7 14 21) {
  add; add;
  assert(stack) {is 42;};
};
