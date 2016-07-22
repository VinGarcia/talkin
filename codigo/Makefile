
.ONESHELL:

# options for 'CLASS' are:
# charClass, strClass, blockClass, arrayClass, objectClass, cRotulo, banco for now.
CLASS=charClass
NUMBER="ALL"

CC    =  g++
DEBUG = -g -fno-inline
FLAGS = -Wall -std=c++11 $(DEBUG)
OBJS =  ambiente.o padrao.o banco.o vars.o
BIN  =  talkin
MAIN  =  main.cpp

# Module files:
SUB_OBJS=$(addprefix code-parser/exp-parser/, shunting-yard.o packToken.o functions.o)
SUB_OBJS+=$(addprefix code-parser/, code-parser.o generators.o)
OBJS += $(SUB_OBJS)

FLAGS+= $(addprefix -I./, $(dir $(SUB_OBJS)))

$(BIN): $(OBJS)
	@echo ""
	@echo " - Compilando \"$(BIN)\" -"
	@echo ""
	@$(CC) $(FLAGS) $(OBJS) $(MAIN) -o $(BIN)

%.o: %.cpp %.hpp
	@echo ""
	@echo " - Compilando Objeto \"$@\" -"
	@echo ""
	@$(CC) $(FLAGS) $< -c

$(SUB_OBJS):
	@echo
	@echo "Checking submodules..."
	@make -C $(dir %@)/
	@echo

number:
	@echo "#define NUMBER $(NUMBER)" > testes/number.h

run: $(BIN)
	xterm ./$(BIN)

safe_run: $(BIN)
	xterm ./$(BIN)

testes/catch.o:
	g++ $(FLAGS) -c testes/catch.cpp -o testes/catch.o

test: number $(OBJS) testes/catch.o
	@rm -f Test.tmp
	@g++ $(FLAGS) $(OBJS) testes/catch.o testes/$(CLASS).cpp -o Test.tmp
	./Test.tmp

clean:
	@rm -f $(BIN) Test.tmp *.o
	@export TERM=xterm
	@clear
