TESTCASE_DIR := pretest-lab2
TESTCASES = $(wildcard $(TESTCASE_DIR)/*.c)
LLFILES = $(patsubst $(TESTCASE_DIR)/%.c,$(TESTCASE_DIR)/%.ll,$(TESTCASES))

.SECONDARY: $(LLFILES)

autograde: $(patsubst $(TESTCASE_DIR)/%.c, $(TESTCASE_DIR)/%.output, $(TESTCASES))

$(TESTCASE_DIR)/%.ll: $(TESTCASE_DIR)/%.c a.out 
	./a.out $< $@

$(TESTCASE_DIR)/%.output: $(TESTCASE_DIR)/%.ll lib.ll
	llvm-link $< lib.ll -S -o $(TESTCASE_DIR)/out.ll
	lli $(TESTCASE_DIR)/out.ll > $@

a.out: lex.yy.o y.tab.o main.c main.o slp.o util.o print_slp.o interp.o table.o
	cc -g main.o slp.o util.o y.tab.o lex.yy.o print_slp.o interp.o table.o

lib.ll: libsysy.c libsysy.h
	clang -S -emit-llvm libsysy.c -o lib.ll -O0

main.o: main.c slp.h slp.c util.h util.c print_slp.h print_slp.c
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

print_slp.o: print_slp.c print_slp.h slp.h
	cc -g -c print_slp.c

table.o: table.c table.h util.h
	cc -g -c table.c

interp.o: interp.c interp.h slp.h table.h
	cc -g -c interp.c

clean: 
	rm -f a.out *.o lex.yy.c y.tab.c y.tab.h y.output lib.ll $(TESTCASE_DIR)/*.ll $(TESTCASE_DIR)/*.output