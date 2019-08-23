push 35;
push 7; 
call +[Int Int];
check({cp;}) {is 42;};

push 35;
push 7;
dispatch +;
check({cp;}) {is 42;};

push 49;
push 7; 
call -[Int Int];
check({cp;}) {is 42;};

push 7;
push 6; 
call *[Int Int];
check({cp;}) {is 42;};

push 42;
push 6; 
call /[Int Int];
check({cp;}) {is 7;};

push 0.1;
push 0.01;
dispatch >;
check({cp;}) {};

push -0.1;
push 0.1;
dispatch <;
check({cp;}) {};

push 0.123;
push 10;
dispatch *;
check({cp;}) {is 1.230;};

push 0.123;
cp;
dispatch +;
check({cp;}) {is 0.246;};

do {
  let foo 41.9;
  step foo 0.1;
  check({cp;}) {is 42.0;};
};
