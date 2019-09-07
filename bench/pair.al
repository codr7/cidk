defconst REPS 10000000;

clock REPS {
  push 1:2:3:4:5:6:7;
  drop;   
};

dump;
