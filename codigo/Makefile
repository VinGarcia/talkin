
.ONESHELL:

# options for 'CLASS' are:
# charClass, strClass, blockClass and arrayClass for now.
CLASS=charClass
OP="ALL"

CC    =  g++
FLAGS = -Wall -std=c++11
OBJS  =  ambiente.o padrao.o banco.o vars.o
MAIN  =  talkin

$(MAIN): $(OBJS)
	@echo ""
	@echo " - Compilando \"$(MAIN)\" -"
	@echo ""
	@$(CC) $(FLAGS) $(OBJS) -o $(MAIN)

%.o: %.cpp %.hpp
	@echo ""
	@echo " - Compilando Objeto \"$@\" -"
	@echo ""
	@$(CC) $(FLAGS) $< -c

number:
	@echo "#define NUMBER $(OP)" > testes/number.h

run: $(MAIN)
	./$(MAIN)

safe_run: $(MAIN)
	xterm ./$(MAIN)

test: number
	@g++ ambiente.cpp padrao.cpp vars.cpp banco.cpp testes/$(CLASS).cpp -o teste -Wall -std=c++11
	./teste

clean:
	@rm -f $(MAIN) *.o
	@export TERM=xterm
	@clear

