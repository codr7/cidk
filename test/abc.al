push (1 2 3);
push (1 2 3);
assert({cp 1;}) {dispatch =;};

push 7;
push 14; 
dispatch <;
assert({cp;}) {is T;};

push 7;
push 14; 
dispatch >;
assert({cp;}) {is F;};

push "foo\"bar";
push "foo\"bar";
assert() {dispatch =;};

do {
  defconst foo 42;
  assert(foo) {is foo 42;};
};

do {
  defconst foo 42;
  do {assert(foo) {is foo 42;};};
};

if T 42 T;
assert({cp;}) {is 42;};

if F 42 7;
assert({cp;}) {is 7;};

do {
  defun foo(_ Int)(Int) { cp; dispatch +; };
  push 21;
  call foo[Int];
  assert({cp;}) {is 42;};
};

do {
  defun foo(_ Int)() { step $ -1; cp; if $ {recall;} _; };
  push 42;
  call foo[Int];
  assert({cp;}) {is 0;};
};
