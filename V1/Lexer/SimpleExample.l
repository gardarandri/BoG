
%{
#include "stdio.h"
%}

/* Here we put definitions */

DIGIT	[0-9]
NUMBER	{DIGIT}+
DELIM	[^0-9]

%%

{NUMBER}	{}

%%
