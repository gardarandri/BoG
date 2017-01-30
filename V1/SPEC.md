
Language Specification
======================

The language has the following keywords:

* int
* float
* bool
* if
* else
* while (Gera þetta?)
* const
* DERV
* TODO: Er þetta fullkominn listi?

The following characters are reserved for special use:

* {
* }
* (
* )
* [
* ]
* ;
* :
* "
* #
* =
* .
* ,
* \

The following operators:

* +
* -
* \*
* /
* %
* ||
* &&
* ==
* !=
* !
* <
* >
* <=
* >=
* +=
* -=
* \*=
* /=
* %=
* ++
* --

Integer constants which the compiler understands follow the regular expression:

> \[0-9\]+

Float constants which the compiler understands follow the regular expression:

> \[0-9\]+(\\.\[0-9\]+)?(\[Ee\]\[+-\]?\[0-9\]+)?

Boolean constants which the compiler understands follow the regular expression:

> (true)|(false)

Names follow the following regular expression:

> \[a-zA-Z\_\]\[a-zA-Z0-9\_\]\*

TODO: Add context-free specification for expressions, if's, for's, while's etc


