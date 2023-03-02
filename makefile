a.out: lex.yy.o y.tab.o main.c main.o slp.o util.o
	cc -g main.o slp.o util.o y.tab.o lex.yy.o -ll

main.o: main.c slp.h slp.c util.h util.c
	cc -g -c main.c

lex.yy.c: lexer.lex y.tab.h y.tab.c
	lex lexer.lex
	
y.tab.c: parser.yacc slp.h 
	yacc -d parser.yacc
	
y.tab.h: parser.yacc
	yacc -d parser.yacc

slp.o: slp.c slp.h util.h
	cc -g -c slp.c

util.o: util.c util.h
	cc -g -c util.c

lex.yy.o: lex.yy.c y.tab.h
	cc -g -c lex.yy.c

y.tab.o: y.tab.c
	cc -g -c y.tab.c

clean: 
	rm -f a.out lex.yy.o lex.yy.c y.tab.o y.tab.c y.tab.h util.o slp.o main.o 
