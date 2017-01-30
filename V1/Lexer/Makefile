FLPATH = "C:\Program Files (x86)\GnuWin32\lib"
LDFLAGS = -lfl

default: compileLexer
	gcc -L$(FLPATH)  -o BoG_Lexer lex.yy.c -D BOG_DEBUG $(LDFLAGS)

compileLexer:
	flex BoG_Lexer.l

test: default
	BoG_Lexer < ..\ExampleProgram.bog

clean:
	del lex.yy.c BoG_Lexer.exe


