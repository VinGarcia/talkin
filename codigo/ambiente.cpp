#include <fstream>
#include <iostream>
#include <stdlib.h>

#include "ambiente.hpp"

using namespace std;
using namespace ambiente;

// Variáveis globais
vars::cObject ambiente::global;
// Drivers do talkin:
std::map<std::string, driver*> ambiente::drivers;

void inicializa_banco()
{
  using namespace pMatch;
  
  // Instruções básicas do sistema:
  //banco::instrucoes[0][3]= cInst("anything: [!-~ \n\t]*",3);
// TODO: fazer ele reconhecer a instrução abaixo usando o objeto "anything"
  banco::instrucoes[0][2]=
  cInst("[!-~ \n\t]* => #!stdout: Comando nao reconhecido.\n  Atualmente so existem os comandos: 'ola', 'help' e 'exit'",2);
  banco::instrucoes[0][1]= cInst("blank: [ \t\n]*",1);
  //banco::instrucoes[0][0]=
  //cInst("#!talkin:[~-! \n\t]* => #!stdout: teste", 0);
  //cInst("#!talkin:(blank)(\"add\",\"rem\",\"show\")(blank)(anything) => #!stdout: teste", 0);
}

void readVocab(ifstream& file)
{
  bool reading=false;
  string line;
  string inst;
  cInst* aux;
  
  try {
    while(getline(file,line))
    {
      if(!reading && line != string("")) reading = true;
      if(reading) inst += " " + line;
      if(line==string(""))
      {
        try{
          aux = banco::addInst(inst);
        }catch(const char* c)
        {throw std::make_pair(c,line);}
        cout << "inst: " << aux->str();
        cout << endl << endl;
        inst = string("");
      }
    }
    if(inst != string(""))
      try{
        aux = banco::addInst(inst);
      }catch(const char* c)
      {throw std::make_pair(c,line);}
    cout << "inst: " << aux->str();
    cout << endl << endl;
  }
  catch(std::pair<const char*,std::string> c) {
    cout << c.first << endl;
    cout << "line: " << c.second << endl;
    throw "Erro no código do arquivo vocabulario.talk";
  }
}

// Retorna true se o contexto é válido, e false caso contrário.
void interface()
{
  string line;
  
  cout << "Talkin 0.9 by Vinícius Garcia" << endl;
  
  while(1)
  {
    getline(cin, line);
    if(line==string("exit")) break;
    
    try{
    banco::execInst(line);
    }catch(const char* c){ cout << string("erro: ") + c << endl; }
  }
}




