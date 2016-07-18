#include <map>

#ifndef AMBIENTE
#define AMBIENTE

#include "banco.hpp"
#include "vars.hpp"

// TODO: These 3 functions below should be moved to main.cpp
// or to a new library file.

// Used to read a .talk file and add it to instruction pool.
void readVocab(std::ifstream& file);

// An interactive loop for the end user.
void interface();

void inicializa_banco();

#endif
