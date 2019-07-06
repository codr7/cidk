fun: foo(Int)(Int) { dup; call +; };

push 21;
call foo;
dump;
