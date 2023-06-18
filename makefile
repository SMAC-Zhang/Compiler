TESTCASE_DIR = tests
INCLUDE_DIR = ./include
SRC_DIR = ./src
FMJ_TESTCASES = $(wildcard $(TESTCASE_DIR)/*.fmj)

testllvm: $(patsubst $(TESTCASE_DIR)/%.fmj, $(TESTCASE_DIR)/%.ll, $(FMJ_TESTCASES))
testrpi: $(patsubst $(TESTCASE_DIR)/%.fmj, $(TESTCASE_DIR)/%.s, $(FMJ_TESTCASES))

$(TESTCASE_DIR)/%.s: $(TESTCASE_DIR)/%.fmj RPI
	@echo TEST $*
	@./RPI $<
	@arm-linux-gnueabihf-gcc -mcpu=cortex-a72 $(TESTCASE_DIR)/$*.s $(TESTCASE_DIR)/libsysy.a --static -o $(TESTCASE_DIR)/$*.out 
	@qemu-arm -B 0x1000 $(TESTCASE_DIR)/$*.out; echo $$?

$(TESTCASE_DIR)/%.ll: $(TESTCASE_DIR)/%.fmj LLVM lib.ll
	@echo TEST $*
	@./LLVM $<
	@llvm-link $(TESTCASE_DIR)/$*.ll lib.ll -S -o $(TESTCASE_DIR)/$*.out
	@lli $(TESTCASE_DIR)/$*.out; echo $$?

OBJ = $(SRC_DIR)/assem.o $(SRC_DIR)/assemblock.o $(SRC_DIR)/bg.o $(SRC_DIR)/canon.o $(SRC_DIR)/fdmjast.o \
		  $(SRC_DIR)/flowgraph.o $(SRC_DIR)/graph.o $(SRC_DIR)/ig.o $(SRC_DIR)/liveness.o $(SRC_DIR)/pr_linearized.o \
		  $(SRC_DIR)/pr_tree_readable.o $(SRC_DIR)/printast.o $(SRC_DIR)/printtreep.o $(SRC_DIR)/semantic.o $(SRC_DIR)/symbol.o\
		  $(SRC_DIR)/table.o $(SRC_DIR)/temp.o $(SRC_DIR)/translate.o $(SRC_DIR)/treep.o $(SRC_DIR)/types.o $(SRC_DIR)/util.o\
		  $(SRC_DIR)/y.tab.o $(SRC_DIR)/lex.yy.o

RPI_OBJ = $(SRC_DIR)/irp2rpi.o $(SRC_DIR)/regalloc.o

LLVM_OBJ = $(SRC_DIR)/codegen.o $(SRC_DIR)/ssa.o

RPI: $(RPI_OBJ) $(OBJ) main.c
	@cc -g -c main.c -I$(INCLUDE_DIR) -I$(SRC_DIR)
	@cc -g $(RPI_OBJ) $(OBJ) main.o -o $@
	@rm -f $(RPI_OBJ) $(OBJ) main.o

LLVM: $(LLVM_OBJ) $(OBJ) main.c
	@cc -g -c -DLLVM $(SRC_DIR)/translate.c -I$(INCLUDE_DIR) -o $(SRC_DIR)/translate.o
	@cc -g -c -DLLVM $(SRC_DIR)/assem.c -I$(INCLUDE_DIR) -o $(SRC_DIR)/assem.o
	@cc -g -c -DLLVM main.c -I$(INCLUDE_DIR) -I$(SRC_DIR)
	@cc -g $(LLVM_OBJ) $(OBJ) main.o -o $@
	@rm -f $(LLVM_OBJ) $(OBJ) main.o

lib.ll: $(SRC_DIR)/libsysy.c $(INCLUDE_DIR)/libsysy.h
	@clang -S -emit-llvm $(SRC_DIR)/libsysy.c -I$(INCLUDE_DIR) -o lib.ll -O0

$(SRC_DIR)/y.tab.o: $(SRC_DIR)/y.tab.c $(SRC_DIR)/y.tab.h
	@cc -g -c $< -I$(INCLUDE_DIR) -o $@

$(SRC_DIR)/lex.yy.o: $(SRC_DIR)/lex.yy.c $(SRC_DIR)/y.tab.h
	@cc -g -c $< -I$(INCLUDE_DIR) -o $@

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE_DIR)/%.h
	@cc -g -c $(SRC_DIR)/$*.c -I$(INCLUDE_DIR) -o $@

$(SRC_DIR)/lex.yy.c: $(SRC_DIR)/lexer.lex $(SRC_DIR)/y.tab.c
	@lex -o $@ $(SRC_DIR)/lexer.lex

$(SRC_DIR)/y.tab.c: $(SRC_DIR)/parser.yacc $(INCLUDE_DIR)/fdmjast.h
	@yacc -do $@ $(SRC_DIR)/parser.yacc

clean:
	rm -f *.o $(SRC_DIR)/*.o RPI LLVM y.output lib.ll $(TESTCASE_DIR)/*.out $(TESTCASE_DIR)/*.ll $(TESTCASE_DIR)/*.s $(TESTCASE_DIR)/*.ast $(TESTCASE_DIR)/*.irp $(TESTCASE_DIR)/*.stm $(TESTCASE_DIR)/*.liv $(TESTCASE_DIR)/qemu*

fclean:
	rm -f  $(TESTCASE_DIR)/*.out $(TESTCASE_DIR)/*.ll $(TESTCASE_DIR)/*.s $(TESTCASE_DIR)/*.ast $(TESTCASE_DIR)/*.irp $(TESTCASE_DIR)/*.stm $(TESTCASE_DIR)/*.liv