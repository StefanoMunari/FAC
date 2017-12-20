# FAC
F Academic Compiler - project developed for the course formal languages and 
compilers at the University of Padua (AY 2016-2017)

## F Language
The F Language is a simple programming language that allows users to
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
C compilator (e.g., gcc). 

For more details you can check 
[the doc repository of FAC](https://github.com/StefanoMunari/FAC-docs)



# License
This software is released under the perl artistic license (reported in the file LICENSE).

# Author 
Mirko Bez

Stefano Munari
