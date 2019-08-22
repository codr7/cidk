defconst REPS 10;
defconst N 100000;

clock REPS {
    let x 0.0;
    
    for N F {
      step x 0.1; 
      drop;
    };
};

dump;

clock REPS {
    push 0.0;
    
    for N F { 
      push 0.1; 
      call +[Fix Fix];
    };

    drop;
};

dump;
