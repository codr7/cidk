clock 1000 {
  include "abc.al";
  include "env.al";
  include "fun.al";
  include "iter.al";
  include "math.al"; 
  include "stack.al";
  include "type.al";
};

dump;
dump stack;
mark; 
sweep;
