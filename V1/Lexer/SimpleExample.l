
%{
#include <stdio.h>

int linenum = 2;
%}

/* Here we put definitions */


%%

if			printf("IF");
else		printf("ELSE");
" "		printf(" space ");
\n			printf("~newline~\n%d. ", linenum++);

%%
