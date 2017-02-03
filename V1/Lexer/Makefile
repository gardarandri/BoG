FLPATH = "C:\Program Files (x86)\GnuWin32\lib"
LDFLAGS = -lfl
EXAMPLEPATH = "ExampleProgram.bog"
FLEXGENFILE = BoG_Lexer_Generated.c

default: compileLexer
	gcc -L$(FLPATH)  -o BoG_Lexer $(FLEXGENFILE) -D BOG_DEBUG $(LDFLAGS)

compileLexer:
	flex -o$(FLEXGENFILE) BoG_Lexer.l

test: default
	BoG_Lexer < $(EXAMPLEPATH)

clean:
	del $(FLEXGENFILE) BoG_Lexer.exe


