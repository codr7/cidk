push 35;
push 7; 
call +[Int Int];
check({cp;}) {is 42;};

push 49;
push 7; 
dispatch -;
check({cp;}) {is 42;};

push 7;
push 6; 
call *[Int Int];
check({cp;}) {is 42;};

push 42;
push 6; 
call /[Int Int];
check({cp;}) {is 7;};


push 7;
push 2;
call ^[Int Int];
check({cp;}) {is 49;};

push 49;
call sqrt[Int];
check({cp;}) {is 7;};

    
push 0.75;
push 0.25; 
call +[Fix Fix];
check({cp;}) {is 1.00;};

push 0.75;
push 0.25;
dispatch -;
check({cp;}) {is 0.50;};

push 2.0;
push 0.25; 
call *[Fix Fix];
check({cp;}) {is 0.5;};

push 1.00;
push 0.25; 
call /[Fix Fix];
check({cp;}) {is 4.00;};


push 0.07000;
cp;
call *[Fix Fix];
check({cp;}) {is 0.00490;};

push 0.0490;
call sqrt[Fix];
check({cp;}) {is 0.2213;};


push 0.07000;
push 2;
call ^[Fix Int];
check({cp;}) {is 0.00490;};


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
