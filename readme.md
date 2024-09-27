# TINY - Tokenised Interpreter for Non-complex Yield

## What is TINY.exe?
Tiny is a JIT interpreter coded in C++ for a custom basic arithmetic programming language - TINY (`*.tiny`).

## What is TINY.tiny?
Files with the `*.tiny` extension are written in the TINY programming language. This language allows for basic arithmetic, assigning numerical values to variables `vint` and constants `cint`, and direct commands to the TINY interpreter (ex. `exit`). 

## Showcase:
(link)

---

# TINY.exe docs
`TINY.exe` takes one line of `TINY.tiny` code and outputs one of the following:
- `output` - the entered line produced an output
- `error` - the entered line contains an unexpected token
- `info` - the interpreter confirms what it's done if the entered line didn't produce any output or error (ex. variable creation)

It then waits for another command. This loop continues until the user enters the `exit` command, which terminates the process.

### Logs:
`TINY.exe` creates a `tiny.log` file every time you open the program. The log is saved in the same directory as `TINY.exe`. The logs contain all user-entered lines and TINY.exe responses ordered chronologically.

### Interpreting a `*.tiny` file:
Go into the directory where TINY.exe is stored. Right-click and select 'Open in Terminal'. Type `TINY.exe your_filename.tiny`. `TINY.exe` will execute the file line-by-line and automatically all. It's like an automated typing in `TINY.tiny` code.

---
# TINY.tiny docs
The TINY.tiny programming language consists of a tiny set of valid characters - 43: numbers, lowercase letters of the English alphabet, and the following symbols: "+-*/= ".

## Command list:
### Interpreter Commands (IC):
`hello` - outputs `Hello, World!`



`exit` - terminates the process

### Syntax:
The language permits loose whitespace unless it interferes with a declaration or produces an invalid token:

```

  rename     foo  // Valid: renames the interpreter to 'foo'

  re name f oo    // Invalid: throws a syntax error

```

---

If the user enters a CI, the line should not contain any other tokens:

```
hello     // Valid: outputs 'Hello, World!'
hello 42  // Invalid: throws an error
```

---

Simple math is valid syntax:

```
6 + 2            // Valid: outputs '8'

  6     +  2       // Valid: outputs '8'

6+2              // Invalid: throws an error
  
6                // Valid: outputs '6'
  
6 + 2 + 3        // Invalid: throws an error
```

---

Creating a variables and constants:

- variable and constant names have to contain only valid lowercase letters
- variables and constants must be assigned an initial value
- variables have to conform to the min and max values of -2,147,483,648 to 2,147,483,647
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
last = 5     // outputs 'last = 5'
last         // outputs '5'

delete last  // throws an error
vint last    // throws an error
```

Renaming the variables and constants:
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
- results have to conform to the min and max values of -2,147,483,648 to 2,147,483,647
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
- comments are not parsed and can contain any string
- the line must start with the keyword `comment`
- all comments are inserted into the log file `tiny.log`

```
comment this is a comment              // outputs 'comment'
vint x = -8 comment variable creation  // throws an error
```