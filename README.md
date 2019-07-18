![Logo](logo.png)
  
```
defun fib(n Int)(Int) {
  if {lt n 2;} n {
    dec n;
    call fib;
    dec n;
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
$ ./cidk ../test/run.al
()
$ rlwrap ./cidk
cidk v0.3

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

(... 42)
```

### Opcodes

#### add x y z*
Adds arguments and pushes result. `x` and `y` are popped from stack if missing.

```
  push 14;
  add 7 $ 21;

(... 42)
```

#### call fun+
Calls functions in specified order. `fun` is popped from stack if missing.

```
  push 7 14;
  call <;

(... T)
```

#### cp [offs 1] [len 1]
Copies `nvals` items starting at `offs` to end of stack.

```
  push 1 2 3;
  cp;

(... 1 2 3 3)
```

```
  push 1 2 3 4 5;
  cp 4 3;

(... 1 2 3 4 5 2 3 4)
```

Passing `T` starts at the beginning and/or copies to end.

```
  push 1 2 3;
  cp T T;

(... 1 2 3 1 2 3)
```

#### defconst [id val]+
Defines compile time constants for pairs of ids and values. `id` and `val` are popped from stack if missing.

```
  do-env {
    defconst foo 35 bar 7;
    push foo bar;
    add;
  };
  
(... 42)

  dump foo;

Error in 'test.al' on row 1, col 5:
Unknown id: foo
```

#### do-env in? body
Evaluates body in environment. Passing `_` creates a new empty environment, while `T` creates one derived from the current.

`env` evaluates to the current environment.

```
  do-env _ {push env;};
  
(... ())
```

#### do-stack in? body
Evaluates body on stack. Passing `_` creates a new empty stack.

```
  do-stack (35 7) {add;};

(... 42)
```

`stack` evaluates to the current stack.

```
  do-stack (1 2) {push 3 4 stack 5 6;};

(... (1 2 3 4) 5 6)
```

#### dump val+
Dumps values to `stderr`. `val` is popped from stack if missing.

#### include file+
Includes the contents of specified files in the current compilation.

The following example includes the main test suite, which prints the number if milliseconds it took to run.

```
  include "../test/run.al";

39
()
```

#### is x y z*
Pushes `T` if all arguments are the same value, `F` otherwise. `x` and `y` are popped from stack if missing.

```
  push 42; is $ 42;

(... T)
```

#### lt x y
Pushes `T` if `x` is less than `y`, otherwise `F`. `x` and `y` are popped from stack if missing.

```
  lt 7 14;

(... T)
```

#### mark
Marks non-reachable references for sweeping.

#### push val+
Pushes values on stack.

#### poke val+
Updates values on stack.

```
  push 1 2 3;
  poke 42;

(... 1 2 42)
```

`_` may be used to skip values, arguments are matched from end of stack.

```
  push 1 2 3;
  poke 42 _;

(... 1 42 3)
```

Expressions are evaluated with current value pushed on stack.

```
  push 1 2 3;
  poke {mul 21;} _;

(... 1 42 3)
```

#### sweep
Sweeps non-marked references.

### License
MIT