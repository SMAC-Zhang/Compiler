TESTCASE_DIR := tests
TESTCASES = $(wildcard $(TESTCASE_DIR)/*.c)
LLFILES = $(patsubst $(TESTCASE_DIR)/%.c,$(TESTCASE_DIR)/%.ll,$(TESTCASES))

.SECONDARY: $(LLFILES)

compile: $(patsubst $(TESTCASE_DIR)/%.c, $(TESTCASE_DIR)/%_compile.output, $(TESTCASES))
interp: $(patsubst $(TESTCASE_DIR)/%.c, $(TESTCASE_DIR)/%_interp.output, $(TESTCASES))


$(TESTCASE_DIR)/%.ll: $(TESTCASE_DIR)/%.c compile.out
	@echo TEST $*
	@./compile.out $< $@

$(TESTCASE_DIR)/%_compile.output: $(TESTCASE_DIR)/%.ll $(TESTCASE_DIR)/%.exp lib.ll
	@llvm-link $< lib.ll -S -o $(TESTCASE_DIR)/out.ll
	@lli $(TESTCASE_DIR)/out.ll > $@
	@diff $@ $(word 2,$^)
	@echo PASS $*
	@echo

$(TESTCASE_DIR)/%_interp.output: $(TESTCASE_DIR)/%.c $(TESTCASE_DIR)/%.exp interp.out
	@echo TEST $*
	@./interp.out $< $@
	@diff $@ $(word 2,$^)
	@echo PASS $*
	@echo

compile.out: lex.yy.o y.tab.o compile_main.c compile_main.o slp.o util.o print_slp.o compile_slp.o table.o
	cc -g compile_main.o slp.o util.o y.tab.o lex.yy.o print_slp.o compile_slp.o table.o -o $@

interp.out: lex.yy.o y.tab.o interp_main.c interp_main.o slp.o util.o print_slp.o interp_slp.o table.o
	cc -g interp_main.o slp.o util.o y.tab.o lex.yy.o print_slp.o interp_slp.o table.o -o $@

lib.ll: libsysy.c libsysy.h
	clang -S -emit-llvm libsysy.c -o lib.ll -O0

compile_main.o: compile_main.c slp.h slp.c util.h util.c print_slp.h print_slp.c compile_slp.h compile_slp.c
	cc -g -c compile_main.c

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

compile_slp.o: compile_slp.c compile_slp.h slp.h table.h
	cc -g -c compile_slp.c

interp_slp.o: interp_slp.c interp_slp.h slp.h table.h
	cc -g -c interp_slp.c

clean: 
	rm -f *.out *.o lex.yy.c y.tab.c y.tab.h y.output lib.ll $(TESTCASE_DIR)/*.ll $(TESTCASE_DIR)/*.output