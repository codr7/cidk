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

### Intro
Welcome! This project aims to implement a development kit for stack based interpreters with an integrated assembler. While most virtual machines resemble physical CPU's to some degree; Cidk is designed to cater to the specific needs of dynamic interpreters, and therefore operates at a higher level.

### License
MIT