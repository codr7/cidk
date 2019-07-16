![Logo](logo.png)
  
```
defun fib(_ Int)(Int) {
  if {dup; lt $ 2;} _ {
    dec; dup;
    call fib;
    swap; dec;
    call fib;
    add;
  };
};
```

### Intro
Welcome to cidk! This project aims to implement a virtual stack machine devkit with an integrated high-level assembler in C++.

### Setup
cidk requires CMake and a C++17-capable compiler to build.

```
$ git clone https://github.com/codr7/cidk.git
$ cd cidk
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./cidk test/run.al
$ ./cidk
cidk v0.1

Press Return on empty row to evaluate.
Evaluating nothing clears stack and Ctrl+D exits.

  push 7 14 21;
  
(7 14 21)
  add;
  
(7 35)
  add;

(42)
```

### Syntax
Each statement starts with an opcode and ends with semicolon, arguments are separated by whitespace.

```
  push 35 7;
  add;

[... 42]
```

### Opcodes

#### add x y z*
Adds arguments and pushes result. `x` and `y` are popped from stack if missing.

```
  push 14;
  add 7 $ 21;

[... 42]
```

#### call fun+
Calls functions in specified order. `fun` is popped from stack if missing.

```
  push 7 14;
  call <;

[... T]
```

#### define [id val]+
Defines compile time constants for pairs of ids and values. `id` and `val` are popped from stack if missing.

```
  do-env {
    define foo 35 bar 7;
    push foo bar;
    add;
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
  do-stack (35 7) {add;};

[... 42]
```

`stack` evaluates to the current stack.

```
  do-stack (1 2) {push 3 4 stack 5 6;};

[... (1 2 3 4) 5 6]
```

#### dump val+
Dumps values to `stderr`. `val` is popped from stack if missing.

#### is x y z*
Pushes `T` if all arguments are the same value, `F` otherwise. `x` and `y` are popped from stack if missing.

```
  push 42; is $ 42;

[... T]
```

#### lt x y
Pushes `T` if `x` is less than `y`, otherwise `F`. `x` and `y` are popped from stack if missing.

```
  lt 7 14;

[... T]
```

#### mark
Marks non-reachable references for sweeping.

#### push val+
Pushes values on stack.

#### sweep
Sweeps non-marked references.

### License
MIT