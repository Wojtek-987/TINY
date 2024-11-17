# TINY - Tokenised Interpreter for Non-complex Yield

## What is TINY.exe?
Tiny is a JIT interpreter coded in C++ for a custom basic arithmetic programming language - TINY (*.tiny).

## What is TINY.tiny?
Files with the `*.tiny` extension are written in the TINY programming language. This language allows for basic arithmetic, assigning numerical values to variables `vint` and constants `cint`, and direct commands to the TINY interpreter (e.g., `exit`).

### Showcase:
[YouTube Link](https://www.youtube.com/@wojtek987)

---

## Before you use
If you encounter any problems running `TINY.exe`:

Download the `DEPENDENCY LIBRARIES` folder and extract those files directly into the directory in which `TINY.exe` is located.

---

## TINY.exe docs

`TINY.exe` takes one line of `TINY.tiny` code and outputs one of the following:

- **Output** - the entered line produced an output.
- **Error** - the entered line contains an unexpected token or invalid syntax.
- **Info** - the interpreter confirms what it's done if the entered line didn't produce any output or error (e.g., variable creation).

It then waits for another command. This loop continues until the user enters the `exit` command, which terminates the process.

---

### Logs:
`TINY.exe` creates a `logs/tiny-<time_stamp>.log.txt` file every time you open the program. The log is saved in the `logs/` directory next to `TINY.exe`. The logs contain all user-entered lines and `TINY.exe` responses ordered chronologically, with timestamps. This is useful for debugging or working with `*.tiny` files.

---

### Interpreting a `*.tiny` file:
Go into the directory where `TINY.exe` is stored. Right-click and select 'Open in Terminal'. Type `TINY.exe "absolute/path/to/your/file.tiny" final`. `TINY.exe` will read the file line-by-line and automatically execute all commands. It's like an automated typing in `TINY.tiny` code. The `final` argument is optional. It will terminate the process after completing file execution. If it's not included, `TINY.exe` will continue to prompt for more commands.

---

## TINY.tiny docs

The `TINY.tiny` programming language consists of a tiny set of valid characters—43 in total: digits, lowercase letters of the English alphabet, and the following symbols: `+`, `-`, `*`, `/`, `=`, and (space included).

---

### Command list:

#### Interpreter Commands (IC):
- `hello` - outputs `hello world!`.
- `clear` - clears the memory of all variables and constants, clears the `tiny-<time_stamp>.log.txt` file, and resets `last` to `0`.
- `exit` - terminates the process.
- `help` - opens the browser with an online reference to docs for language usage and compiler docs.

---

### Syntax:

The language permits loose whitespace unless it interferes with a declaration or produces an invalid token:

```plaintext
vint     foo  =    6  // Valid: creates a variable named `foo` and assigns it the value `6`.

vi nt f oo    // Invalid: Error: Missing or invalid identifier.
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

 6     +  2      // Valid: outputs '8'

6+2              // Invalid: Error: Invalid numeric value '6+2'.

6                // Valid: outputs '6'

6 + 2 + 3        // Invalid: Error: Unexpected input after math operation.
```

---

Creating variables and constants:

- variable and constant names have to contain only valid lowercase letters
- variables and constants must be assigned an initial value
- variables and constants have to conform to the min and max values of `-2,147,483,648` to `2,147,483,647`
```
// Variable:
vint foo = 5   // Outputs 'foo = 5'

// Constant:
cint bar = 1   // Outputs 'bar: 1'

// Invalid:
vint 2 = 0     // Invalid: Error: Invalid identifier '2'.

// Redefining:
vint foo = 2   // Invalid: Error: Could not create variable 'foo'.

// No value:
vint x         // Invalid: Error: Missing '=' after identifier.
```

Accessing variables and constants:
```
foo  // Outputs '5'
bar  // Outputs '1'
```

Changing the values of variables and constants:
```
foo = -3  // Outputs 'foo = -3'
bar = -3  // Error: Cannot modify constant 'bar'. (no change)
```

Setting the value of a variable to the last numerical output - the `last` reserved keyword:
```
bar          // Outputs 'bar: 1'
last         // Outputs '1'
foo = last   // Outputs 'foo = 1'

last         // Outputs '1'
last = 5     // Error: Cannot modify 'last'.

last         // Outputs '1'

delete last  // Error: Cannot delete 'last'.
vint last    // Error: 'last' is a reserved keyword.
```

Renaming variables and constants:
```
rename foo = foobar // Outputs 'foo -> foobar = -3'
rename bar = tiny   // Outputs 'bar -> tiny: 1'
```

Deleting variables and constants:
```
delete foobar // Outputs 'foo -> #'
delete tiny   // Outputs 'tiny -> #'
```

---

Math on variables and constants:
- limited to 2 tokens separated by an operation
- results have to conform to the min and max values of `-2,147,483,648` to `2,147,483,647`
- division results in integers (floored)

```
vint a = 10    // Outputs 'a = 10'
cint b = 2     // Outputs 'b: 2'

a - b          // Outputs '8'

vint c = last  // Outputs 'c = 8'

a + b - c      // Invalid: Error: Unexpected input after math operation.

// Division:
cint x = 5  // Outputs 'x: 5'
cint y = 2  // Outputs 'y: 2'

x / y       // Outputs '2' (floored from 2.5)
```

---

Comments:
- comments are not parsed but still have to use only valid characters
- the line must start with the keyword `comment`
- all comments are inserted into the log file `logs/tiny-<time_stamp>.log.txt`

```
comment This is a comment              // Outputs 'comment: This is a comment '
vint x = -8 comment variable creation  // Invalid: Error: Unexpected token: comment
```

---

# Example `TINY.tiny` file: (blank lines will pause the program)
```
comment tiny script showcase
comment hello world
hello

comment create variables and constants
vint a = 10
cint b = 5

comment perform a math operation
a + b

comment assign last to a new variable
vint c = last

comment output the value of c
c

comment change the value of a
a = 20

comment subtract b from a
a - b

comment rename c to total
rename c = total

comment output total
total

comment delete a variable
delete a

comment attempt to access deleted variable
a

comment use last keyword
last

comment clear memory
clear
last

comment attempt an invalid math operation
total + b + 5

comment exit the program
exit
```