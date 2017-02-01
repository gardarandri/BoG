FLPATH = "C:\Program Files (x86)\GnuWin32\lib"
LDFLAGS = -lfl
EXAMPLEPATH = "..\ExampleProgram.bog"

default: compileLexer
	gcc -L$(FLPATH)  -o BoG_Lexer BoG_Lexer.c -D BOG_DEBUG $(LDFLAGS)

compileLexer:
	flex -oBog_Lexer.c BoG_Lexer.l

test: default
	BoG_Lexer < $(EXAMPLEPATH)

clean:
	del BoG_Lexer.cpp BoG_Lexer.exe


