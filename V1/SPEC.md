
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

program 	= fundcel , ";" , program
			| vardecl , ";" , program
			| expr , ";" , program


fundecl		= "fun" , NAME , "(" , args , ")" , body

args		= "" 
			| NAME 
			| NAME , "," , args

vardecl		= "var" , vdt

vdt1 		= NAME , vdt2

vdt2		= "=" , expr , vdt3

vdt3		= ""
			| "," , vdt1

expr		= "return" , expr
			| NAME , "=" , expr
			| orexpr

orexpr		= andexpr , orexprt

orexpr1		= "||" , orexpr
			| ""

andexpr		= notexpr , andexprt

andexprt	= "&&" , andexpr
			| ""

notexpr		= "!" , notexpr
			| opexpr

opexpr		= smallexpr , opexprt

opexprt		= OPNAME , opexpr
			| ""

smallexpr	= NAME , smallexprn
			| OPNAME , expr
			| LITERAL
			| "(" , expr , ")"
			| ifexpr
			| "while" , "(" , expr , ")" , body
			| fundecl

smallexprn	= ""
			| "(" , manyexpr , ")"

manyexpr	= expr , manyexprt

manyexprt	= "," , manyexpr
			| ""

ifexpr		= "if" , "(" , expr , ")" , body , ifexprt1

ifexprt1	= ""
			| "elseif" , "(" , expr , ")" , body , ifexprt2

ifexprt2	= ""
			| "else" , body

body		= "{" , innerbody , "}"

innerbody	= funcdecl , ";" , innerbodyt
			| vardecl , ";" , innerbodyt
			| expr , ";" , innerbodyt

innerbodyt	= ""
			| funcdecl , ";" , innerbodyt
			| vardecl , ";" , innerbodyt
			| expr , ";" , innerbodyt
