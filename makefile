TESTCASE_DIR := tests
FMJ_TESTCASES = $(wildcard $(TESTCASE_DIR)/*.fmj)
C_TESTCASES = $(wildcard $(TESTCASE_DIR)/*.c)

testc: $(patsubst $(TESTCASE_DIR)/%.c, $(TESTCASE_DIR)/%.output1, $(C_TESTCASES))

testfmj: $(patsubst $(TESTCASE_DIR)/%.fmj, $(TESTCASE_DIR)/%.output2, $(FMJ_TESTCASES))

$(TESTCASE_DIR)/%.output1: $(TESTCASE_DIR)/%.c main
	@echo TEST $*
	@./main < $< > $@

$(TESTCASE_DIR)/%.output2: $(TESTCASE_DIR)/%.fmj main
	@echo TEST $*
	@./main < $< > $@

main: main.o prog1.o util.o printast.o fdmjast.o y.tab.o lex.yy.o types.o symbol.o table.o type_check.o
	@cc -g $^ -o $@

main.o: main.c prog1.c util.h util.c printast.h printast.c fdmjast.h fdmjast.c y.tab.c y.tab.h types.h types.c symbol.h symbol.c table.h table.c type_check.h type_check.c 
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

type_check.o: type_check.c type_check.h util.h symbol.h table.h fdmjast.h types.h
	@cc -g -c type_check.c

printast.o: printast.c printast.h fdmjast.h
	@cc -g -c printast.c

clean: 
	rm -f *.o main lex.yy.c y.tab.c y.tab.h y.output lib.ll $(TESTCASE_DIR)/*.output $(TESTCASE_DIR)/*.output2