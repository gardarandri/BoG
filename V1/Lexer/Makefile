FLPATH = "C:\Program Files (x86)\GnuWin32\lib"
LDFLAGS = -lfl
EXAMPLEPATH = "ExampleProgram.bog"
FLEXGENFILE = BoG_Lexer_Generated.c
LEXERTESTER = BoG_Lexer_Test

default: compileLexer
	gcc -L$(FLPATH)  -o BoG_Lexer_Test $(LEXERTESTER) -D BOG_DEBUG $(LDFLAGS)

compileLexer:
	flex -o$(FLEXGENFILE) BoG_Lexer.l

debug: default
	BoG_Lexer_Test < $(EXAMPLEPATH)

build: clean compileLexer
	gcc -L$(FLPATH)  -o BoG_Lexer_Test -DYY_MAIN=0 BoG_Lexer_Test.c $(LDFLAGS)

test: build
	BoG_Lexer_Test.exe < ExampleProgram.bog

clean:
	del $(FLEXGENFILE) BoG_Lexer_Test.exe


