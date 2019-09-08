push (1 2 3);
push (1 2 3);
check({cp 1;}) {dispatch =;};

push 7;
push 14; 
dispatch <;
check({cp;}) {is T;};

push 7;
push 14; 
dispatch >;
check({cp;}) {is F;};

push "foo\"bar";
push "foo\"bar";
check() {dispatch =;};

do {
  defconst foo 42;
  check(foo) {is foo 42;};
};

do {
  defconst foo 42;
  do {check(foo) {is foo 42;};};
};

if T 42 T;
check({cp;}) {is 42;};

if F 42 7;
check({cp;}) {is 7;};

do {
  defun foo(_:Int)(Int) { cp; dispatch +; };
  push 21;
  call foo[Int];
  check({cp;}) {is 42;};
};

do {
  defun foo(_:Int)() { step $ -1; cp; if $ {recall;} _; };
  push 42;
  call foo[Int];
  check({cp;}) {is 0;};
};

do {
    defer {push 3;};
    defer {push 2;};
    push 1;
}; stash; check({cp;}) {push (1 2 3); dispatch =;};
