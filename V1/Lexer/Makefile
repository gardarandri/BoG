FLPATH = "C:\Program Files (x86)\GnuWin32\lib"
LDFLAGS = -lfl
EXAMPLEPATH = "ExampleProgram.bog"
FLEXGENFILE = BoG_Lexer_Generated.c

default: compileLexer
	gcc -L$(FLPATH)  -o BoG_Lexer $(FLEXGENFILE) -D BOG_DEBUG $(LDFLAGS)

compileLexer:
	flex -o$(FLEXGENFILE) BoG_Lexer.l

debug: default
	BoG_Lexer < $(EXAMPLEPATH)

build: clean compileLexer
	gcc -L$(FLPATH)  -o BoG_Lexer -DYY_MAIN=0 BoG_Lexer.c $(LDFLAGS)

test: build
	BoG_Lexer.exe < ExampleProgram.bog

clean:
	del $(FLEXGENFILE) BoG_Lexer.exe


