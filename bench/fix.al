defconst REPS 10;
defconst N 100000;

clock REPS {
    let x 0.0;
    
    for N F { 
      set x {push 0.1; call +[Fix Fix];};
    };    
};

dump;

clock REPS {
    push 0.0;
    
    for N F { 
      push 0.1; 
      call +[Fix Fix];
    };
};

dump;
