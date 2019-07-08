defun foo(_ Int)(Int) { dup; call +; };

push 21;
call foo;
dump;
