
#include "stdio.h"
#include "BoG_Parser.h"


int main(int argc, const char** argv){
	if (argc == 1)
	{
		printf("%s: Fatal: No input file!\n", argv[0]);
		exit(11);
	}

	parse(argv[1]);

	printf("Lexing and parsing was successful!\n");
}
