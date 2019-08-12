push 1 2 3; cp; stash;
assert({cp;}) {eq (1 2 3 3);};

push 1 2 3 4 5; cp 3 3; stash;
assert({cp;}) {eq (1 2 3 4 5 2 3 4);};

push 1 2 3; cp T T; stash;
assert({cp;}) {eq (1 2 3 1 2 3);};

push 1 2 3; swap 0 1; stash;
assert({cp;}) {eq (1 3 2);};

push 1 2 3; swap 1; stash;
assert({cp;}) {eq (1 3 2);};

do-env {
  let foo 1 bar 2;
  swap foo bar;
  push foo bar;
  stash;
  assert({cp;}) {eq (2 1);};
};

do-env {
  let foo 1;
  push 2;
  swap foo;
  push foo;
  stash;
  assert({cp;}) {eq (1 2);};
};

push 1 2 3;
set 1 4;
stash;
assert({cp;}) {eq (1 4 3);};

do-env {
  let foo 1;
  set foo 2;
  assert(foo) {eq foo 2;};
};

do-env {
  let foo 1;
  set foo {push 41; call-bin +;};
  assert(foo) {eq foo 42;}; 
};

push 42 7; poke 35 _; call-bin +;
assert({cp;}) {is 42;};

push 1 2 3; poke {push 2; call-bin *;} _; stash;
assert({cp;}) {eq (1 4 3);};
