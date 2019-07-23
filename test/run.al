clock 1000 {include "abc.al" "iter.al" "stack.al";};

dump;
dump stack;
mark; sweep;
