
.ONESHELL:

# options for 'CLASS' are:
# charClass, strClass, blockClass, arrayClass, objectClass, cRotulo, banco for now.
CLASS=charClass
NUMBER="ALL"

CC    =  g++
FLAGS = -Wall -std=c++11 -g
OBJS =  ambiente.o padrao.o banco.o vars.o
BIN  =  talkin
MAIN  =  main.cpp

# Subdirectories:
VPATH=exp-parser
MODULES=exp-parser
FLAGS+=-I./exp-parser
OBJS+=exp-parser/shunting-yard.o exp-parser/packToken.o

$(BIN): $(MODULES) $(OBJS)
	@echo ""
	@echo " - Compilando \"$(BIN)\" -"
	@echo ""
	@$(CC) $(FLAGS) $(OBJS) $(MAIN) -o $(BIN)

%.o: %.cpp %.hpp
	@echo ""
	@echo " - Compilando Objeto \"$@\" -"
	@echo ""
	@$(CC) $(FLAGS) $< -c

exp-parser:
	@make -C exp-parser/

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
