push 1 2 3; cp; stash;
assert({cp;}) {push (1 2 3 3); dispatch =;};

push 1 2 3 4 5; cp 3 3; stash;
assert({cp;}) {push (1 2 3 4 5 2 3 4); dispatch =;};

push 1 2 3; cp -1 -1; stash;
assert({cp;}) {push (1 2 3 1 2 3); dispatch =;};

push 1 2 3; swap 0 1; stash;
assert({cp;}) {push (1 3 2); dispatch =;};

push 1 2 3; swap 1; stash;
assert({cp;}) {push (1 3 2); dispatch =;};

do-env {
  let foo 1 bar 2;
  swap foo bar;
  push foo bar;
  stash;
  assert({cp;}) {push (2 1); dispatch =;};
};

do-env {
  let foo 1;
  push 2;
  swap foo;
  push foo;
  stash;
  assert({cp;}) {push (1 2); dispatch =;};
};

push 1 2 3;
set 1 4;
stash;
assert({cp;}) {push (1 4 3); dispatch =;};

do-env {
  let foo 1;
  set foo 2;
  assert(foo) {push foo 2; dispatch =;};
};

do-env {
  let foo 1;
  set foo {push 41; dispatch +;};
  assert(foo) {push foo 42; dispatch =;}; 
};

push 42 7; poke 35 _; dispatch +;
assert({cp;}) {is 42;};

push 1 2 3; poke {push 2; dispatch *;} _; stash;
assert({cp;}) {push (1 4 3); dispatch =;};
