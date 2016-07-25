#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdexcept>
#include <exception>

#include "ambiente.hpp"
#include "shunting-yard.h" // For Function class
#include "shunting-yard-exceptions.h" // For msg_exception class

using namespace std;

void inicializa_banco() {
  using namespace pMatch;
  
  // Instruções básicas do sistema:
  //banco::instrucoes[0][3]= cInst("anything: [!-~ \n\t]*",3);
// TODO: fazer ele reconhecer a instrução abaixo usando o objeto "anything"
  banco::instrucoes[0][2]=
  cInst("\"[!-~ \n\t]*\" { print(\"Comando nao reconhecido.\\n  Atualmente so existem os comandos: 'ola', 'help' e 'exit'\") }",2U);
  banco::instrucoes[0][1]= cInst("blank: \"[ \t\n]*\"",1U);
  //banco::instrucoes[0][0]=
  //cInst("#!talkin:[~-! \n\t]* => #!stdout: teste", 0);
  //cInst("#!talkin:(blank)(\"add\",\"rem\",\"show\")(blank)(anything) => #!stdout: teste", 0);
}

struct Startup_talkin {
  // This singleton adds the talkin() function to the global namespace
  // This happens before main() gets executed.
  Startup_talkin() {
    // Add talkin function to global scope:
    Scope::default_global()["talkin"] = CppFunction(&talkin, 1, args);
  }

  const char* args[1] = {"text"};
  static packToken talkin(const Scope* scope) {
    // Get a single argument:
    std::string text = scope->find("text")->asString();
    banco::execInst(text);
    return packToken::None;
  }
} startup_talkin;

void readVocab(ifstream& file) {
  bool reading=false;
  string line;
  string inst;
  cInst* aux;
  
  try {
    while (getline(file,line)) {
      if (!reading && line != string("")) reading = true;
      if (reading) inst += " " + line;
      if (line==string("")) {
        try {
          aux = banco::addInst(inst);
        } catch (const char* c) {
          throw std::make_pair(c,line);
        }
        cout << "inst: " << aux->str();
        cout << endl << endl;
        inst = string("");
      }
    } if (inst != string("")) {
      try {
        aux = banco::addInst(inst);
      } catch (const char* c) {
        throw std::make_pair(c,line);
      } catch (const exception& e) {
        throw std::make_pair(e.what(), line);
      }
    }
    cout << "inst: " << aux->str();
    cout << endl << endl;
  }
  catch (std::pair<const char*,std::string> c) {
    cout << c.first << endl;
    cout << "line: " << c.second << endl;
    throw "Erro no código do arquivo vocabulario.talk";
  }
}

// Retorna true se o contexto é válido, e false caso contrário.
void interface() {
  uint pos;
  string line;
  
  cout << "Talkin v0.9a by Vinícius Garcia" << endl;
  
  while (true) {
    getline(cin, line);
    if (line == "exit") break;

    // Skip blank lines:
    pos = 0;
    while (isblank(line[pos])) ++pos;
    if (line[pos] == '\0') continue;
    
    try {
      banco::execInst(line);
    } catch (const char* c) {
      cout << string("erro: ") + c << endl;
    } catch (const std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
}




