push 1:2:3:4:5:6:7;
check({cp;}) {is 1:2:3:4:5:6:7;};

do {
  let a:b 1:2:3;
  
  push a;
  check({cp;}) {is 1;};

  push b;
  check({cp;}) {is 2:3;};
};

do {
  let a:b {push 1:2;}:3;

  push a;
  check({cp;}) {is 1:2;};

  push b;
  check({cp;}) {is 3;};
};

splat 1:2:3;
stash;
check({cp;}) {push (1 2 3); dispatch =;};

splat 1:2:3 1;
stash;
check({cp;}) {push (1 2:3); dispatch =;};
