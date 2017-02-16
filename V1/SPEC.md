
Language Specification
======================

The language has the following keywords:

* var
* if
* else
* while
* return
* fun

The following characters are delimiters:

* (
* )
* {
* }
* ;
* .
* ,


The strings that are of this form are operators:

> \[+-/*%<>=|\]+

Integer constants which the compiler understands follow the regular expression:

> \[0-9\]+

Float constants which the compiler understands follow the regular expression:

> \[0-9\]+(\\.\[0-9\]+)?(\[Ee\]\[+-\]?\[0-9\]+)?

Boolean constants which the compiler understands follow the regular expression:

> (true)|(false)

String constanst follow the regular expression:

> \"(\[^"\n\r\f])*\"

Names follow the following regular expression:

> \[a-zA-Z\_\]\[a-zA-Z0-9\_\]\*

The following EBNF is the syntax spesification

~~~~
program 	= function , program
			| function

function	= NAME , "(" , args , ")" , "{" , { decl , ";" } , { expr , ";" } , "}"

decl		= "var" , NAME , { NAME }

expr		= NAME
			| NAME , "=" , expr
			| NAME , "(" , [ expr , { "," , expr } ] , ")"
			| "return" , expr
			| OPNAME , expr
			| expr OPNAME expr
			| LITERAL
			| "(" , expr , ")"
			| ifexpr
			| "while" , "(" , expr , ")" , body

ifexpr		= "if" , "(" , expr , ")" , body , [ "elif" , "(" , expr , ")" , body ] , [ "else" , body ]

body		= "{" , expr , ";" , { expr , ";" } , "}"
~~~~
