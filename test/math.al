push 35 7; call +[Int Int];
assert({cp;}) {is 42;};

push 49 7; call -[Int Int];
assert({cp;}) {is 42;};

push 7 6; call *[Int Int];
assert({cp;}) {is 42;};

push 42 6; call /[Int Int];
assert({cp;}) {is 7;};

push 6 4 3;
dispatch + *;
assert({cp;}) {is 42;};
