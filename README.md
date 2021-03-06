![Logo](logo.png)
  
### Intro
cidk is an interpreter devkit with integrated assembler written in C++.

```
defun fib(n:Int)(Int) {
  push n;
  push 2;
  dispatch <;

  if $ n {
    step n -1;
    dispatch fib;
    step n -1;
    dispatch fib;
    dispatch +; 
  };
};
```

### Setup
cidk requires CMake and a C++17 compiler to build.

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
```

```
$ rlwrap ./cidk

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
cidk typically runs faster than Python3, 2-20x as far as available [benchmarks](https://github.com/codr7/cidk/tree/master/bench) go.

### Syntax
Each operation starts with an opcode and ends with semicolon, arguments are separated by whitespace.

### Types
The following types are supported by default.

#### Meta
`Meta` is the type of all types, including itself.

`typeof` returns the type of the specified value.

```
  typeof Meta;

(... Meta:Meta)
```

#### Nil
`Nil` represents missing values.

```
  typeof _;

(... Nil:Meta)
```

Suffixing any type id except `Nil` with `?` gives its nil type, that matches the specified type or nil.

```
  dump Meta?;
  
(... Meta?:Meta)
```
```
  isa Meta Nil;

(... _)
```
```
  isa Meta? Nil;

(... Meta?:Meta)
```

#### A
`A` is the parent of all types except `Nil`.

`isa` returns the direct parent type, or `_` if none exists.

```
  isa A Int;

(... Num:Meta)
```

```
  isa A Num;

(... A:Meta)
```

#### Bool
`Bool` has two values, `T` and `F`. All values have boolean representations, most are unconditionally `T`; empty strings and lists are two notable exceptions.

```
  if "" T F;

(... F)
```

#### Int
Integers are 64-bit and signed.

```
  typeof 42;

(... Int:Meta)

  isa Num;

(... Num:Meta)
```

Prefixing integers with `0x` triggers hex decoding.

```
  push 0x2a;

(... 42)
```

#### Fix
Fix point numbers infer precision from literals.

```
  push 41.90;
  step $ 0.1;

(... 42.00)
```

#### Str
Strings are mutable and passed by reference. Literals are enclosed in double quotes and assumed to be UTF8-encoded.

```
  is "foo" "foo";

(... F)
```

```
  push "foo";
  push "foo";
  call =[A A];

(... T)
```

#### Pairs
Values may be paired using `:`.

```
  42:"foo"

(... 42:"foo")
```

`let` supports destructuring.

```
  do {
    let a:b 1:2:3;
    push a;
    push b;
  };

(... 1 2:3)
```

Braces may be used to control evaluation order.

```
  do {
    let a:b {push 1:2}:3;
    push a;
    push b;
  };

(... 1:2 3)
```

#### List
Lists are heterogenous, mutable and passed by reference. Literals are enclosed in parens.

```
  typeof (42 "foo");

(... List:Meta)
```

#### Fun
Functions are generic; the same id may be used for multiple definitions as long as their argument lists are unique.

```
  push +;
  
(... (+[Int Int]:Fun +[Fix Fix]:Fun))
```

```
  push 1;
  push 2;
  call +[Int Int];
  
(... 3)
```

```
  push 1 2;
  dispatch +;

(... 3)
```

### Opcodes
Included below is an alphabetical list of all supported opcodes.

#### call [fun $] [check F]
Calls `fun`, arguments are type checked if `check` is `T`.

```
  push 35;
  push 7;
  call +[Int Int] T;

(... 42)
```

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

#### defer expr
Schedules `expr` to be undconditionally evaluated in reverse order on scope exit.

```
  do {
    defer {dump 3;};
    defer {dump 2;};
    dump 1;
    fail "Going down...";
  };

1
2
3
Error at row 5, col 6:
Going down...
()
```

#### defconst id val
Defines compile time constant named `id`.

```
  do {
    defconst foo 42;
    dump foo;
  };

42
(...)

  dump foo;

Error in 'test.al' on row 1, col 5:
Unknown id: foo
```

#### dispatch fun [nargs -1]
Calls most specific implementation for specified function based on the last `nargs` values on stack.

```
  push 35;
  push 7;
  dispatch +;

(... 42)
```
An error is thrown when no matching function can be found.

```
  push "foo";
  push 42;
  dispatch +;
  
Error at row 2, col 9:
Dispatch failed
("foo" 42)
```

#### do body
Evaluates body in a fresh environment.

```
  do {let foo 42; push foo;};

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
Dumps `val` to `stderr`.

```
  dump "hello";

"hello"
(...)

```

#### fail msg
Signal error with message.

```
  fail "Going down...";

Error at row 1, col 4:
Going down...
()
```

#### include fname
Inlines the specified file in the current compilation unit.

The following example includes the main test suite, which prints the number if milliseconds it took to run a thousand repetitions when evaluated.

```
  include "../test/run.al";

39
()
```

#### is [x $] [y $]
Pushes `T` if `x` and `y` are the same value;

```
  push 42;
  is 42;

(... T)
```

#### isa [parent $] [child $]
Pushes the direct parent type, or `_` if none exists.

```
  isa A Int;

(... Num:Meta)
```

#### let id [val $]
Binds `id` to `val` in the current scope.

```
  do {
    let foo 42;
    push foo;
  };

(... 42)
```

#### link fname
Links and initializes the specified plugin.

```
  link "/usr/local/lib/libcalcl.so";
  calc(35 + 7);

(... 42)
```

#### mark
Marks non-reachable references for sweeping.

#### push val
Pushes `val` on stack.

#### set key [val $]
Updates value for `key` to `val`.

Integers index the stack from end.

```
  push 1 2 3;
  set 1 4;

(... 1 4 3)
```

While symbols act on the environment.

```
  do {
    let foo 1;
    push foo;
    set foo 2;
    push foo;
  };

(... 1 2)
```

Expressions are evaluated with current value pushed on stack.

```
  do {
    let foo 1;
    set foo {push 41; dispatch +};
    push foo;
  };

(... 42)
```

#### step place [delta 1] [push T]
Steps value at `place` by `delta` and pushes result if `push` is `T`.

```
  do {
    let foo 41.9;
    step foo 0.1;
  };

(... 42.0)
```

#### swap [x 1] [y 0]
Swaps values of `x` and `y`.

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
  do {
    let foo 1 bar 2;
    swap foo bar;
    push foo;
    push bar;
  };

(... 2 1)
```

Mixing is fine too.

```
  do {
    let foo 1;
    push 2;
    swap foo;
    push foo;
  };

(... 1 2)
```

#### sweep
Sweeps non-marked references.

#### typeof [val $]
Pushes the type of `val`.

```
  typeof 42;

(... Int:Meta)
```

### Plugins
A cidk plugin is a shared library that exports a function with the following signature:

```
void cidk_pluginit(Cx &cx, const Pos &)
```

Plugins may be linked and initialized using `link`. The following example loads the `calc` opcode from [calcl](https://github.com/codr7/calcl) dynamically

```
  link "/usr/local/lib/libcalcl.so";
  calc(35 + 7);

(... 42)
```

### License
MIT

### support
Please consider donating if you would like to help the project evolve and improve.

<a href="https://liberapay.com/codr7/donate"><img alt="Donate using Liberapay" src="https://liberapay.com/assets/widgets/donate.svg"></a>