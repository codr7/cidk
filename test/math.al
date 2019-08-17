push 35;
push 7; 
call +[Int Int];
assert({cp;}) {is 42;};

push 35;
push 7;
dispatch +;
assert({cp;}) {is 42;};

push 49;
push 7; 
call -[Int Int];
assert({cp;}) {is 42;};

push 7;
push 6; 
call *[Int Int];
assert({cp;}) {is 42;};

push 42;
push 6; 
call /[Int Int];
assert({cp;}) {is 7;};
