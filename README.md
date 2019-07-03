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
Welcome to cidk! This project aims to implement a stack machine devkit with integrated assembler in C++. While most virtual machines resemble physical CPUs to some degree; cidk is designed to cater to the needs of dynamic languages, and therefore operates at a higher level.

### License
MIT