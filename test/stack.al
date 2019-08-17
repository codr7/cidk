push 1;
push 2;
push 3; 
cp; 
stash;
check({cp;}) {push (1 2 3 3); dispatch =;};

push 1;
push 2;
push 3;
push 4;
push 5; 
cp 3 3; 
stash;
check({cp;}) {push (1 2 3 4 5 2 3 4); dispatch =;};

push 1;
push 2;
push 3; 
cp -1 -1; 
stash;
check({cp;}) {push (1 2 3 1 2 3); dispatch =;};

push 1;
push 2;
push 3; 
swap 0 1; 
stash;
check({cp;}) {push (1 3 2); dispatch =;};

push 1;
push 2;
push 3; 
swap 1; 
stash;
check({cp;}) {push (1 3 2); dispatch =;};

do {
  let foo 1;
  let bar 2;
  swap foo bar;
  push foo;
  push bar;
  stash;
  check({cp;}) {push (2 1); dispatch =;};
};

do {
  let foo 1;
  push 2;
  swap foo;
  push foo;
  stash;
  check({cp;}) {push (1 2); dispatch =;};
};

push 1;
push 2;
push 3;
set 1 4;
stash;
check({cp;}) {push (1 4 3); dispatch =;};

do {
  let foo 1;
  set foo 2;
  check(foo) {push foo; push 2; dispatch =;};
};

do {
  let foo 1;
  set foo {push 41; dispatch +;};
  check(foo) {push foo; push 42; dispatch =;}; 
};
