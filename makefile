TESTCASE_DIR := tests
FMJ_TESTCASES = $(wildcard $(TESTCASE_DIR)/*.fmj)
C_TESTCASES = $(wildcard $(TESTCASE_DIR)/*.c)

testfmj: $(patsubst $(TESTCASE_DIR)/%.fmj, $(TESTCASE_DIR)/%.output, $(FMJ_TESTCASES))
#testline: $(patsubst $(TESTCASE_DIR)/%.fmj, $(TESTCASE_DIR)/%.line, $(FMJ_TESTCASES))

$(TESTCASE_DIR)/%.output: $(TESTCASE_DIR)/%.fmj main
	@echo TEST $*
	@./main friendly $(TESTCASE_DIR)/$*.irp $(TESTCASE_DIR)/$*.line $(TESTCASE_DIR)/$*.llir < $< > $(TESTCASE_DIR)/$*.graph

# $(TESTCASE_DIR)/%.line: $(TESTCASE_DIR)/%.fmj main
# 	@echo TEST $*
# 	@./main friendly tests/error.output < $< > $@

canon.o: canon.c canon.h
	@cc -g -c canon.c

printtreep.o: printtreep.c printtreep.h
	@cc -g -c printtreep.c

pr_linearized.o: pr_linearized.c pr_linearized.h
	@cc -g -c pr_linearized.c

main: main.o prog1.o util.o printast.o fdmjast.o y.tab.o lex.yy.o types.o symbol.o table.o semantic.o \
	treep.o pr_tree_readable.o translate.o temp.o printtreep.o pr_linearized.o canon.o assem.o codegen.o \
	assemblock.o bg.o graph.o liveness.o flowgraph.o ig.o
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

codegen.o: codegen.h codegen.c
	@cc -g -c codegen.c

assem.o: assem.h assem.c
	@cc -g -c assem.c

assemblock.o: assemblock.h assemblock.c
	@cc -g -c assemblock.c

bg.o: bg.h bg.c
	@cc -g -c bg.c

flowgraph.o: flowgraph.h flowgraph.c
	@cc -g -c flowgraph.c

graph.o: graph.c graph.h
	@cc -g -c graph.c

ig.o: ig.c ig.h
	@cc -g -c ig.c

liveness.o: liveness.c liveness.h
	@cc -g -c liveness.c

clean: 
	rm -f *.o main y.output lib.ll $(TESTCASE_DIR)/*.output $(TESTCASE_DIR)/*.line $(TESTCASE_DIR)/*.irp $(TESTCASE_DIR)/*.llir $(TESTCASE_DIR)/*.graph $(TESTCASE_DIR)/*.bg

bclean:
	rm -f *.o main y.output lib.ll