![Logo](logo.png)
  
```
fun fib(Int)(Int) {
  dup;
    
  if {push 2; call <;} _ {
    call dec; dup;
    call fib; swap;
    call dec fib +;
  };
};

bench 10 10 {push 20; call fib;};
dump;
```

### License
MIT