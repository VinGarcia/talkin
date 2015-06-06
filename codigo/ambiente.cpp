#include <fstream>
#include <iostream>
#include <stdlib.h>

#include "ambiente.hpp"

using namespace std;
using namespace ambiente;

// Variáveis globais
vars::cVar ambiente::global;
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




/* * * * * START TEST cContexto * * * * */

// Teste do cContexto:
// Descomente para testar:
 /*
int main(int argc, char* argv[])
{
  using namespace pMatch;
  cContexto con;
  int pos;
  
  cout << " * * * * * TESTE cContexto() * * * * *\n\n";
  
  cout << "  @Testes com cContextos`s:" << endl << endl;
  
  //banco::addInst("(\"[0-9][0-9]*\")p; - p = 3 => #!stdout: works!");
  //banco::execInst("3");
  
  try{
  cout << 1 << endl;
  cout << "Teste \"0 = 0\"" << endl;
  con = cContexto("0 = 0", pos=0);
  cout << "      \"" << con.str() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 1.1 << endl;
  cout << "Teste \"10 = 10\"" << endl;
  con = cContexto("10 = 10", pos=0);
  cout << "      \"" << con.str() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 1.2 << endl;
  cout << "Teste \"10 = 10\"" << endl;
  con = cContexto("10= 10", pos=0);
  cout << "      \"" << con.str() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 1.3 << endl;
  cout << "Teste \"10 = 10\"" << endl;
  con = cContexto("10 =10", pos=0);
  cout << "      \"" << con.str() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 1.4 << endl;
  cout << "Teste \"10 = 10\"" << endl;
  con = cContexto(" 10 = 10", pos=0);
  cout << "      \"" << con.str() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 1.5 << endl;
  cout << "Teste \"10 = 10\"" << endl;
  con = cContexto("10 = 10 ", pos=0);
  cout << "      \"" << con.str() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 2 << endl;
  cout << "Teste \"0 = 0\"" << endl;
  con = cContexto("\"0\" = 0", pos=0);
  cout << "      \"" << con.str() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 3 << endl;
  cout << "Teste \"var = 0\"" << endl;
  con = cContexto("var = 0", pos=0);
  cout << "      \"" << con.str() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  cout << "      @Teste da função eval():" << endl << endl;

  // Criando variáveis locais:
  vars::cVar v = vars::cVar();
  v.subvars["var"] = vars::cVar();
  v.subvars["var"].nome = "var";
  v.subvars["var"].valor = "0";
  
  try{
  cout << 4 << endl;
  cout << "Teste 1" << endl;
  con = cContexto("var = 0", pos=0);
  cout << "      " << con.eval(v,ambiente::global) << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  v.subvars["var"].valor="1";
  try{
  cout << 4.1 << endl;
  cout << "Teste 0" << endl;
  con = cContexto("var = 0", pos=0);
  cout << "      " << con.eval(v,ambiente::global) << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  return 0;
}
// */

/* * * * * END TEST cContexto * * * * */