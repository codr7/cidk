splat (1 2 (3));
stash;
check({cp;}) {push (1 2 3); dispatch =;};

splat (1 2 (3)) 1;
stash;
check({cp;}) {push (1 2 (3)); dispatch =;};
