
.ONESHELL:

# options for 'CLASS' are:
# charClass, strClass, blockClass and arrayClass for now.
CLASS=charClass
OP="ALL"

CC    =  g++
FLAGS = -Wall -std=c++11
OBJS  =  ambiente.o padrao.o banco.o vars.o
BIN  =  talkin
MAIN  =  main.cpp

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

number:
	@echo "#define NUMBER $(OP)" > testes/number.h

run: $(BIN)
	./$(BIN)

safe_run: $(BIN)
	xterm ./$(BIN)

test: number
	@g++ ambiente.cpp padrao.cpp vars.cpp banco.cpp testes/$(CLASS).cpp -o Test -Wall -std=c++11
	./Test

clean:
	@rm -f $(BIN) Test *.o
	@export TERM=xterm
	@clear

