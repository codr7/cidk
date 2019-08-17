![Logo](logo.png)
  
```
defun fib(n Int)(Int) {
  push n;
  push 2;
  call <[Any Any];

  if $ n {
    step n -1;
    call fib[Int];
    step n -1;
    call fib[Int];
    call +[Int Int]; 
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

  push 7;
  push 14
  push 21;
  
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

### Opcodes

#### call [fun $] [check F]
Calls `fun` or function popped from from stack if missing.

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

Passing `-1` copies from beginning to end.

```
  push 1;
  push 2;
  push 3;
  cp -1 -1;

(... 1 2 3 1 2 3)
```

#### defconst id val
Defines compile time constant named `id`.

```
  do-env {
    defconst foo 42;
    dump foo;
  };

42
(...)

  dump foo;

Error in 'test.al' on row 1, col 5:
Unknown id: foo
```

#### dispatch fun
Calls the most specific implementation for specified function based on stack contents.

```
  push 35;
  push 7;
  dispatch +;

(... 42)
```
An error is thrown if no matching function is found.

```
  push "foo";
  push 42;
  dispatch +;
  
Error in "n/a" at row 2, col 9:
Dispatch failed
("foo" 42)
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
  push 1;
  push 2;
  push 3;
  push 4;
  drop 2;

(... 1 2)
```

#### dump [val $]
Dumps `val`, or value popped from stack if missing, to `stderr`.

```
  dump "hello";

"hello"
(...)

```
#### include file
Inlines the contents of specified file in the current compilation unit.

The following example includes the main test suite, which prints the number if milliseconds it took to run.

```
  include "../test/run.al";

39
()
```

#### is [x $] [y $]
Pushes `T` if `x` and `y` are the same value, otherwise `F`. `x` and `y` are popped from stack if missing.

```
  push 42;
  is 42;

(... T)
```

#### let id [val $]
Binds `id` to `val`, or value popped from stack if missing, in the current scope.

```
  let foo 42;
  push foo;

(... 42)
```

#### mark
Marks non-reachable references for sweeping.

#### push val
Pushes `val` on stack.

#### set key [val $]
Updates value for `key` to `val` or value popped from stack if missing.

Integers index the stack from end.

```
  push 1 2 3;
  set 1 4;

(... 1 4 3)
```

While symbols act on the environment.

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

#### swap [x 1] [y 0]
Swaps value of `x` and `y`.

Integers index the stack from end.

```
  push 1;
  push 2;
  push 3;
  swap 0 1;

(... 1 3 2)
```

While symbols act on the environment.

```
  do-env {
    let foo 1 bar 2;
    swap foo bar;
    push foo;
    push bar;
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