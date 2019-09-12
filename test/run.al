clock 1000 {
  include "abc.al";
  include "env.al";
  include "fun.al";
  include "iter.al";
  include "list.al";
  include "math.al"; 
  include "num.al";
  include "pair.al"; 
  include "stack.al";
  include "type.al";
};

dump;
dump stack;
mark; 
sweep;
