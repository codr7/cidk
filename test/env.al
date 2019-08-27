do {
  let foo 42;

  do {
    do {push foo;};
  };

  check({cp;}) {is 42;};
};

