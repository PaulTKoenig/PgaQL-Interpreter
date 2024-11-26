make: main.c lexer.c parser.c interpreter.c
	gcc -o main main.c lexer.c parser.c interpreter.c -I.