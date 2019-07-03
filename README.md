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

### Opcode Reference

#### call fun+

#### const [id val]+
Defines env-scoped compile time constants.

```
  do-env {
    const foo 35 bar 7;
    push foo bar;
    call +;
  };
  
42

  dump foo;

Error in 'test.al' on row 1, col 5:
Unknown id: foo
```

#### do-env expr
Evaluates expression in separate environment.

#### do-stack expr
Evaluates expression on a fresh stack.

#### dump val+
Dumps values to stderr.

#### push val+
Pushes values on stack.

### License
MIT