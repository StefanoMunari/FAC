#Unit Test

The aim of these unit tests is to test the different components of the
FAC compiler, especially the classes of the c_src directory, using
the [cmocka test framework](https://cmocka.org/).
 
#Compilation
`gcc -L/usr/lib/ test.c <other c files to test> -lcmocka`
