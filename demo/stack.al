push 42;
dump 1 $ 3;

do-stack (35 7) {call +;};
dump;

do-stack (1 2) {push 3 4 stack 5 6;};
dump stack;
