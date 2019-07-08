![Logo](logo.png)
  
```
defun fib(Int)(Int) {
  if {dup; push 2; call <;} _ {
    call dec; dup;
    call fib; swap;
    call dec fib +;
  };
};
```

### Intro
Welcome to cidk! This project aims to implement a stack machine devkit with integrated assembler in C++.

### Setup
cidk requires CMake and a C++17-capable compiler to build.

```
$ git clone https://github.com/codr7/cidk.git
$ cd cidk
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./cidk ../bench/fib_rec.al
```

### Syntax
Each statement starts with an opcode and ends with semicolon, arguments are separated by whitespace.

```
  push 35 7;
  call +;

[... 42]
```

### Opcodes

#### call fun*
Calls functions in specified order, or pops function from stack if no arguments.

```
  push 7 14 21;
  call + <;

[... F]
```

#### define [id val]*
Defines compile time constants for pairs of keys and values, or pops key and value from stack if no arguments.

```
  do-env {
    define foo 35 bar 7;
    push foo bar;
    call +;
  };
  
[... 42]

  dump foo;

Error in 'test.al' on row 1, col 5:
Unknown id: foo
```

#### do-env in? body
Evaluates body in environment. Passing `_` creates a new empty environment, while `T` creates one derived from the current.

`env` evaluates to the current environment.

```
  do-env _ {push env;};
  
[... ()]
```

#### do-stack in? body
Evaluates body on stack. Passing `_` creates a new empty stack.

```
  do-stack (35 7) {call +;};

[... 42]
```

#### dump val*
Dumps values to `stderr`, or pops value from stack if no arguments.

#### push val+
Pushes values on stack.

### License
MIT