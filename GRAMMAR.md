# F Syntax
**Program**
```
<program> ::= <stmt>
```
**Statments**
```
<stmt> ::= epsilon | <stmt> <declaration> <separator> |
		<stmt> <var-assignment> | <stmt> <print_var> |
		<stmt> <ifrule> | <stmt> <whilerule>

<declaration> ::= <type> <id> <assignment-op> <expr>
<var-assignment> ::= <id> <assignment-op> <expr>
<print-var> ::= print(<id>)
<ifrule> ::= if (<expr>) { <stmt> } else { <stmt> } 
| if (<expr>) { <stmt> }
<whilerule>	::= while (<expr>) { <stmt> } 
```
**Expressions**
```
<expr> ::= <expr> <aop> <expr> | + <expr> | - <expr> |
		   (<expr>) | <fract-literal> | <expr> <bop_2> <expr> |
		   <expr> <RELOP> <expr> | <BOP1> <expr> |
		   <boolean-literal> | <id>
```
**Tokens**
```
<type> ::= fract | bool
<aop> ::= \+ | \- | \* | \/
<BOP_2> ::= && | \|\| | -> | <-> | XOR  
<BOP_1> ::= !
<RELOP> ::= < | <= | == | <> | >= | >
<id> ::= [a-zA-Z][a-zA-Z0-9_]*
<fract-literal> ::= [numerator | denominator]