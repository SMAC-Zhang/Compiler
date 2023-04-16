TESTCASE_DIR := tests
FMJ_TESTCASES = $(wildcard $(TESTCASE_DIR)/*.fmj)
C_TESTCASES = $(wildcard $(TESTCASE_DIR)/*.c)

testfmj: $(patsubst $(TESTCASE_DIR)/%.fmj, $(TESTCASE_DIR)/%.output, $(FMJ_TESTCASES))

$(TESTCASE_DIR)/%.output: $(TESTCASE_DIR)/%.fmj main
	@echo TEST $*
	@./main < $< > $@

main: main.o prog1.o util.o printast.o fdmjast.o y.tab.o lex.yy.o types.o symbol.o table.o semantic.o treep.o pr_tree_readable.o translate.o temp.o
	@cc -g $^ -o $@

main.o: main.c prog1.c util.h util.c printast.h printast.c fdmjast.h fdmjast.c y.tab.c y.tab.h types.h types.c symbol.h symbol.c table.h table.c semantic.h semantic.c 
	@cc -g -c main.c

prog1.o: prog1.c fdmjast.h
	@cc -g -c prog1.c

fdmjast.o: util.h fdmjast.c fdmjast.h
	@cc -g -c fdmjast.c

lex.yy.c: lexer.lex y.tab.h y.tab.c
	@lex lexer.lex
	
y.tab.c: parser.yacc fdmjast.h
	@yacc -d parser.yacc
	
y.tab.h: parser.yacc fdmjast.h
	@yacc -d parser.yacc

util.o: util.c util.h
	@cc -g -c util.c

lex.yy.o: lex.yy.c y.tab.h
	@cc -g -c lex.yy.c

y.tab.o: y.tab.c
	@cc -g -c y.tab.c

types.o: types.c types.h symbol.h util.h
	@cc -g -c types.c

symbol.o: symbol.c symbol.h util.h table.h
	@cc -g -c symbol.c

table.o: table.c table.h util.h
	@cc -g -c table.c

semantic.o: semantic.c semantic.h util.h symbol.h table.h fdmjast.h types.h
	@cc -g -c semantic.c

printast.o: printast.c printast.h fdmjast.h
	@cc -g -c printast.c

treep.o: treep.c treep.h util.h symbol.h temp.h fdmjast.h table.h
	@cc -g -c treep.c

pr_tree_readable.o: pr_tree_readable.c pr_tree_readable.h treep.h util.h temp.h symbol.h table.h
	@cc -g -c pr_tree_readable.c

translate.o: translate.c translate.h treep.h util.h symbol.h temp.h table.h
	@cc -g -c translate.c

temp.o: temp.h temp.c table.h symbol.h
	@cc -g -c temp.c

clean: 
	rm -f *.o main lex.yy.c y.tab.c y.tab.h y.output lib.ll $(TESTCASE_DIR)/*.output