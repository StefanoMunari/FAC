# FAC
F Academic Compiler - project developed for the course formal languages and 
compilers at the University of Padua (AY 2016-2017)

## F Language
The F Language is a simple imperative programming language that allows users to
perform mathematical and relational operations on fractions and
simple boolean operations.

### Syntax
The formal syntax of the language is defined in the file [GRAMMAR.md](GRAMMAR.md).
In the following example we will explain intuitively the syntax.

```C

```

## How does FAC works
FAC parses the input F program, creates an Abstract
Syntax Tree (AST), that get rid of grammatic symbols (e.g., `';'`)
and performs type checking on this convenient intermediate representation.
Afterwards it creates a 3-Address Code version of the AST and
put it into an equivalent C File, that can be compiled using a standard
C compiler (e.g., gcc). 

### Documentation

- deps: latex, make

For more details you can check 
[the doc repository of FAC](https://github.com/StefanoMunari/FAC-docs)

### Dev documentation

- deps: doxygen

From the root dir of the project it is possible to generate developers' documentation
```bash
make doc
```

To access the documentation generated in doc/ check in your browser
```bash
doc/index.html
```

### Compile FAC

```bash
make
```

### Write an F program
```C
# FILE: example.f
fract a = [1|2];
fract b = [2|4];

fract c = (a / b) + b;

print(c);
```

### Compile an F program with FAC
```bash
make run ARGS=<path_to_my_file>
# example
make run ARGS=example.f
```
FAC will output the name of the compiled program (in C), which can be compiled with gcc
```bash
gcc <name_of_F_compiled_file>
# example
gcc main.c
```

### Run the compiled program
```bash
./<compiled_C_file>
# example
./a.out
# result
[3|2]
```

# License
This software is released under the perl artistic license (reported in the file LICENSE).

# Author 
Mirko Bez

Stefano Munari
