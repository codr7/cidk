for 3 T {};
stash;
assert({cp;}) {eq (0 1 2);};

for 3 F 42;
stash;
assert({cp;}) {eq (42 42 42);};
