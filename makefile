TESTCASE_DIR := tests
TESTCASES = $(wildcard $(TESTCASE_DIR)/*.fmj)

test: $(patsubst $(TESTCASE_DIR)/%.fmj, $(TESTCASE_DIR)/%.output, $(TESTCASES))

correct: $(patsubst $(TESTCASE_DIR)/%.fmj, $(TESTCASE_DIR)/%.output2, $(TESTCASES))

$(TESTCASE_DIR)/%.output: $(TESTCASE_DIR)/%.fmj main
	@echo TEST $*
	@./main < $< > $@

$(TESTCASE_DIR)/%.output2: $(TESTCASE_DIR)/%.output main
	@echo TEST $*
	@./main < $< > $@
	@diff $@ $(word 1,$^)
	@echo PASS $*
	@echo

main: main.o util.o printast.o fdmjast.o y.tab.o lex.yy.o
	@cc -g $^ -o $@

main.o: main.c util.h util.c printast.h printast.c fdmjast.h fdmjast.c y.tab.c y.tab.h
	@cc -g -c main.c

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

printast.o: printast.c printast.h fdmjast.h
	@cc -g -c printast.c

clean: 
	rm -f *.o main lex.yy.c y.tab.c y.tab.h y.output lib.ll $(TESTCASE_DIR)/*.output $(TESTCASE_DIR)/*.output2