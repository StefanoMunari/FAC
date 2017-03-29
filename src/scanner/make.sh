#!/bin/bash
flex scanner.fl && gcc lex.yy.c scanner_utils.c -lfl && ./a.out $1
