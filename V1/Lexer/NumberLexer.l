
%{
#include <stdio.h>
%}


DIGIT			[0-9]
WNUMBER		{DIGIT}+
WS				[ \n\t]
NUMBER			{WNUMBER}(\.{WNUMBER})?([Ee][+-]?{WNUMBER})?



%%


{NUMBER}	printf("Found number %s\n", yytext);
.			printf("");
\n			printf("");




