# TINY - Tokenised Interpreter for Non-complex Yield

## What is TINY.exe?
Tiny is a JIT interpreter coded in C++ for a custom basic arithmetic programming language - TINY (`*.tiny`).

## What is TINY.tiny?
Files with the `*.tiny` extension are written in the TINY programming language. This language allows for basic arithmetic, assigning numerical values to variables `vint` and constants `cint`, and direct commands to the TINY interpreter (ex. `exit`). 

## Showcase:
[YouTube Link](https://www.youtube.com/@wojtek987)

---

# TINY.exe docs
`TINY.exe` takes one line of `TINY.tiny` code and outputs one of the following:
- `Output` - the entered line produced an output
- `Error` - the entered line contains an unexpected token
- `Info` - the interpreter confirms what it's done if the entered line didn't produce any output or error (ex. variable creation)

It then waits for another command. This loop continues until the user enters the `exit` command, which terminates the process.

### Logs:
`TINY.exe` creates a `logs/tiny-<time_stamp>.log.txt` file every time you open the program. The log is saved in the `logs/` directory next to `TINY.exe`. The logs contain all user-entered lines and TINY.exe responses ordered chronologically, with timestamps. This is useful for debugging or working with `*.tiny` files.

### Interpreting a `*.tiny` file:
Go into the directory where TINY.exe is stored. Right-click and select 'Open in Terminal'. Type `TINY.exe "absolute/path/to/your/file.tiny" final`. `TINY.exe` will read the file line-by-line and automatically execute all commands. It's like an automated typing in `TINY.tiny` code. The `final` argument is optional. It will terminate the process after completing file execution. If it's not included, `TINY.exe` will continue to prompt for more commands.

---
# TINY.tiny docs
The TINY.tiny programming language consists of a tiny set of valid characters - 43: digits, lowercase letters of the English alphabet, and the following symbols: "+-*/= " ([space] included).

## Command list:
### Interpreter Commands (IC):
`hello` - outputs `hello world!`

`clear` - clears the memory of all variables and constants, clears the `tiny-<time_stamp>.log.txt` file, and resets `last` to `0`

`exit` - terminates the process

`help` - opens the browser with an online reference to docs for language usage and compiler docs

### Syntax:
The language permits loose whitespace unless it interferes with a declaration or produces an invalid token:

```

  vint     foo  =    6  // Valid: creates a variable named `foo` and assigns it the value `6`

  vi nt f oo    // Invalid: throws a syntax error

```

---

If the user enters a IC, the line should not contain any other tokens:

```
hello     // Valid: outputs 'hello world!'
hello 42  // Invalid: throws 'Unexpected token: 42'
```

---

Simple math is valid syntax:

```
6 + 2            // Valid: outputs '8'

  6     +  2     // Valid: outputs '8'

6+2              // Invalid: throws an error
  
6                // Valid: outputs '6'
  
6 + 2 + 3        // Invalid: throws an error
```

---

Creating variables and constants:

- variable and constant names have to contain only valid lowercase letters
- variables and constants must be assigned an initial value
- variables and constants have to conform to the min and max values of `-2,147,483,648` to `2,147,483,647`
```
// Variable:
vint foo = 5   // outputs 'foo = 5'

// Constant:
cint bar = 1   // outputs 'bar: 1'

// Invalid:
vint 2 = 0     // throws an error

// Redefining:
vint foo = 2   // throws an error

// No value:
vint x         // throws an error
```

Accessing variables and constants:
```
foo  // outputs '5'
bar  // outputs '1'
```

Changing the values of variables and constants:
```
foo = -3  // outputs 'foo = -3'
bar = -3  // outputs 'bar: 1' (no change)
```

Setting the value of a variable to the last numerical output - the `last` reserved keyword:
```
bar          // outputs '1'
last         // outputs '1'
foo = last   // outputs 'foo = 1'

last         // outputs '1'
last = 5     // throws an error
last         // outputs '1'

delete last  // throws an error
vint last    // throws an error
```

Renaming variables and constants:
```
foo = foobar // outputs 'foo -> foobar = -3'
bar = tiny   // outputs 'bar -> tiny: 1'
```

Deleting variables and constants:
```
delete foobar // outputs 'foo -> #'
delete tiny   // outputs 'tiny -> #'
```

---

Math on variables and constants:
- limited to 2 tokens separated by an operation
- results have to conform to the min and max values of `-2,147,483,648` to `2,147,483,647`
- division results in integers (floored)

```
vint a = 10    // outputs 'a = 10'
cint b = 2     // outputs 'b: 2'

a - b          // outputs '8'

vint c = last  // outputs 'c = 8'

a + b - c      // throws an error


// Division:
cint x = 5  // outputs 'x: 5'
cint y = 2  // outputs 'y: 2'

x / y       // outputs '2' (floored from 2.5)
```

---

Comments:
- comments are not parsed but still have to use only valid characters
- the line must start with the keyword `comment`
- all comments are inserted into the log file `logs/tiny-<time_stamp>.log.txt`

```
comment this is a comment              // outputs 'comment'
vint x = -8 comment variable creation  // throws an error
```

---

# Example `TINY.tiny` file:
```
comment TINY Program to Generate Fibonacci Sequence (Up to 5 Terms)

comment Initialize the first two Fibonacci numbers
vint a = 0
vint b = 1

comment Output the first two numbers
a
b

comment Calculate the next number in the Fibonacci sequence: a + b
a + b
vint fib3 = last
fib3

comment Update values for the next step (b becomes the new a, fib3 becomes the new b)
a = b
b = fib3

comment Calculate the next Fibonacci number
a + b
vint fib4 = last
fib4

comment Update values for the next step
a = b
b = fib4

comment Calculate the next Fibonacci number
a + b
vint fib5 = last
fib5

comment The Fibonacci sequence up to 5 terms: 0, 1, 1, 2, 3
```
