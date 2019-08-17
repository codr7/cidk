![Logo](logo.png)
  
```
defun fib(n Int)(Int) {
  push n 2; call <[Int Int]; if $ n {
    dec n;
    call fib[Int];
    dec n;
    call fib[Int] +[Int Int]; 
  };
};
```

### Intro
Welcome to cidk!<br/>
This project aims to implement an interpreter dev kit with integrated assembler in C++.

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
39
()
$ rlwrap ./cidk
cidk v0.8

Press Return on empty row to evaluate.
Empty input clears stack and Ctrl+D exits.

  push 7 14 21;
  
(7 14 21)
  dispatch +;
  
(7 35)
  dispatch +;

(42)
```

### Performance
cidk currently runs at 2-15x the speed of Python3, as far as available [benchmarks](https://github.com/codr7/cidk/tree/master/bench) go. As an added bonus, launching the interpreter is roughly 200x as fast.

### Syntax
Each statement starts with an opcode and ends with semicolon, arguments are separated by whitespace.

```
  push 35 7;
  dispatch +;

(... 42)
```

### Opcodes

#### call fun+
Calls functions in specified order. `fun` is popped from stack if missing.

#### cp [offs 0] [len 1]
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

Passing `-1` starts at the beginning and copies to end.

```
  push 1 2 3;
  cp -1 -1;

(... 1 2 3 1 2 3)
```

#### defconst [id val]+
Defines compile time constants for pairs of ids and values.

```
  do-env {
    defconst foo 35 bar 7;
    push foo bar;
    dispatch +;
  };
  
(... 42)

  dump foo;

Error in 'test.al' on row 1, col 5:
Unknown id: foo
```

#### dispatch fun+
Calls the most specific function based on stack contents.

```
  push 6 4 3;
  dispatch + *;

(... 42)
```

#### do-env body
Evaluates body in a fresh environment.

```
  do-env {let foo 42; push foo;};

(... 42)
```

#### drop [n 1]
Drops `n` values from stack.

```
  push 1 2 3;
  drop 2;

(... 1)
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
  poke {push 21; dispatch *;} _;

(... 1 42 3)
```
#### set [key val?]+
Updates values on stack or in environment depending on key type. `val` is popped from stack if missing.

```
  push 1 2 3;
  set 1 4;

(... 1 4 3)
```

```
  do-env {
    let foo 1;
    push foo;
    set foo 2;
    push foo;
  };

(... 1 2)
```

Expressions are evaluated with current value pushed on stack.

```
  do-env {
    let foo 1;
    set foo {push 41; dispatch +};
    push foo;
  };

(... 42)
```

#### swap [x y?]?
Swaps values.

Integers index the stack from end.

```
  push 1 2 3;
  swap 0 1;

(... 1 3 2)
```

Default place is end of stack.

```
  push 1 2 3;
  swap 1;

(... 1 3 2)
```

While symbols act on the environment.

```
  do-env {
    let foo 1 bar 2;
    swap foo bar;
    push foo bar;
  };

(... 2 1)
```

Mixing is fine too.

```
  do-env {
    let foo 1;
    push 2;
    swap foo;
    push foo;
  };

(... 1 2)
  
```

#### sweep
Sweeps non-marked references.

### License
MIT