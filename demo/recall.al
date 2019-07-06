fun: foo(Int)() { call dec; dup; if $ {recall;} _; };

push 42;
call foo;
