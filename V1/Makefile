FLPATH = "C:\Program Files (x86)\GnuWin32\lib"
INCLUDE = -I./Lexer/ -I./Parser/

default:
	gcc -L$(FLPATH) $(INCLUDE) -o BoG_Compiler BoG_Compiler.c -lfl

test: default
	./BoG_Compiler ExampleProgram.bog

debugtest:
	gcc -L$(FLPATH) $(INCLUDE) -o BoG_Compiler BoG_Compiler.c -lfl -DLEXER_DEBUG
	./BoG_Compiler ExampleProgram.bog

debug:
	gcc -L$(FLPATH) $(INCLUDE) -o BoG_Compiler BoG_Compiler.c -lfl -DLEXER_DEBUG

clean:
	rm BoG_Compiler
	rm Lexer/BoG_Lexer_Generated.c


