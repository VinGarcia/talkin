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



/* * * * * START TEST cExpressao * * * * */

// Teste do cExpressao:
// Descomente para testar:
 /*
int main(int argc, char* argv[])
{
  cExpressao exp;
  int pos;
  
  cout << " * * * * * TESTE cExpressao() * * * * *\n\n";
  
  cout << "  @Testes com operandos numericos:" << endl << endl;
  
  //banco::addInst("(\"[0-9][0-9]*\")p; - p = 3 => #!stdout: works!");
  //banco::execInst("3");
  
  try{
  cout << 1 << endl;
  cout << "Teste \"0 0 <\"" << endl;
  exp = cExpressao("0 < 0", pos=0);
  cout << "      \"" << exp.str_raw() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 2 << endl;
  cout << "Teste \"0 0 ==\"" << endl;
  exp = cExpressao("0 == 0", pos=0);
  cout << "      \"" << exp.str_raw() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 3 << endl;
  cout << "Teste \"0 0 -\"" << endl;
  exp = cExpressao("0 - 0", pos=0);
  cout << "      \"" << exp.str_raw() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 4 << endl;
  cout << "Teste \"0 1 2 * +\"" << endl;
  exp = cExpressao("0 + 1 * 2", pos=0);
  cout << "      \"" << exp.str_raw() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 5 << endl;
  cout << "Teste \"0 1 * 2 +\"" << endl;
  exp = cExpressao("0 * 1 + 2", pos=0);
  cout << "      \"" << exp.str_raw() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 6 << endl;
  cout << "Teste \"0 1 2 * + 3 + 4 ==\"" << endl;
  exp = cExpressao("0 + 1 * 2 + 3 == 4", pos=0);
  cout << "      \"" << exp.str_raw() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 7 << endl;
  cout << "Teste \"0 1 2 3 * + == 4 +\"" << endl;
  exp = cExpressao("0 == 1 + 2 * 3 + 4", pos=0);
  cout << "      \"" << exp.str_raw() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 8 << endl;
  cout << "Teste \"0 ! 1 ||\"" << endl;
  exp = cExpressao("! 0 || 1", pos=0);
  cout << "      \"" << exp.str_raw() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 9 << endl;
  cout << "Teste \"1 2 3 4 * + ==\"" << endl;
  exp = cExpressao("1 == 2 + 3 * 4", pos=0);
  cout << "      \"" << exp.str_raw() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 10 << endl;
  cout << "Teste \"0 1 * 2 + 3 ==\"" << endl;
  exp = cExpressao("0 * 1 + 2 == 3", pos=0);
  cout << "      \"" << exp.str_raw() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 11 << endl;
  cout << "Teste \"0 1 ! 2 && ||\"" << endl;
  exp = cExpressao("0 || ! 1 && 2", pos=0);
  cout << "      \"" << exp.str_raw() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  cout << "  @Testes com operandos diversas:" << endl << endl;

  try{
  cout << 1 << endl;
  cout << "Teste '\"0\" \"1\" +'" << endl;
  exp = cExpressao("\"0\" + \"1\"", pos=0);
  cout << "      '" << exp.str_raw() << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 2 << endl;
  cout << "Teste '\"um\" \"dois\" +'" << endl;
  exp = cExpressao("\"um\" + \"dois\"", pos=0);
  cout << "      '" << exp.str_raw() << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 3 << endl;
  cout << "Teste 'a b +'" << endl;
  exp = cExpressao("a + b", pos=0);
  cout << "      '" << exp.str_raw() << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 4 << endl;
  cout << "Teste 'a.b c +'" << endl;
  exp = cExpressao("a.b + c", pos=0);
  cout << "      '" << exp.str_raw() << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 5 << endl;
  cout << "Teste 'a.b c.[0] +'" << endl;
  exp = cExpressao("a.b + c.[0]", pos=0);
  cout << "      '" << exp.str_raw() << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 6 << endl;
  cout << "Teste 'a.b.c.d.e[10].batata c.[0] +'" << endl;
  exp = cExpressao("a.b.c.d.e[10].batata + c.[0]", pos=0);
  cout << "      '" << exp.str_raw() << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  cout << "  @Testes avaliando operações com strings:" << endl << endl;

  vars::cVar local, global;

  try{
  cout << 1.1 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("\"batata\" == \"batata\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 1.2 << endl;
  cout << "Teste ''" << endl;
  exp = cExpressao("\"batata\" == \"batatA\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 2.1 << endl;
  cout << "Teste ''" << endl;
  exp = cExpressao("\"batata\" != \"batata\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 2.2 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("\"batata\" != \"Batata\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 3.1 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("\"0\" < \"1\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 3.2 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("\"10\" < \"11\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 3.3 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("\"10.1\" < \"11\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 3.4 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("\"10.1\" < \"11.123\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 3.5 << endl;
  cout << "Teste ''" << endl;
  exp = cExpressao("\"10.1\" < \"10.1\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 3.6 << endl;
  cout << "Teste ''" << endl;
  exp = cExpressao("\"10.1\" < \"-10.1\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 3.7 << endl;
  cout << "Teste ''" << endl;
  exp = cExpressao("\"10.1q\" < \"10.1\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 3.8 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("\"10.1\" < \"10.1q\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 4.1 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("\"1\" > \"0\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 4.2 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("\"11\" > \"10\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 4.3 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("\"11\" > \"10.1\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 4.4 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("\"11.123\" > \"10.1\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 4.5 << endl;
  cout << "Teste ''" << endl;
  exp = cExpressao("\"10.1\" > \"10.1\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 4.6 << endl;
  cout << "Teste ''" << endl;
  exp = cExpressao("\"-10.1\" > \"10.1\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 4.7 << endl;
  cout << "Teste ''" << endl;
  exp = cExpressao("\"10.1\" > \"10.1q\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 4.8 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("\"10.1q\" > \"10.1\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 5.1 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("\"a\" && \"b\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 5.2 << endl;
  cout << "Teste ''" << endl;
  exp = cExpressao("\"a\" && \"\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 5.3 << endl;
  cout << "Teste ''" << endl;
  exp = cExpressao("\"\" && \"\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 6.1 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("\"a\" || \"b\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 6.2 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("\"a\" || \"\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 6.3 << endl;
  cout << "Teste ''" << endl;
  exp = cExpressao("\"\" || \"\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 7.1 << endl;
  cout << "Teste '7'" << endl;
  exp = cExpressao("\"4\" + \"3\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 7.2 << endl;
  cout << "Teste '7.1'" << endl;
  exp = cExpressao("\"7.0\" + \"0.1\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 7.3 << endl;
  cout << "Teste '7.1'" << endl;
  exp = cExpressao("\"7.0\" + \"1e-1\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 7.4 << endl;
  cout << "Teste ''" << endl;
  exp = cExpressao("\"7.0\" + \"1e-1q\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 8.1 << endl;
  cout << "Teste '7'" << endl;
  exp = cExpressao("\"10\" - \"3\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 8.2 << endl;
  cout << "Teste '7.1'" << endl;
  exp = cExpressao("\"7.2\" - \"0.1\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 8.3 << endl;
  cout << "Teste '7.1'" << endl;
  exp = cExpressao("\"7.2\" - \"1e-1\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 8.4 << endl;
  cout << "Teste ''" << endl;
  exp = cExpressao("\"7.2\" - \"1e-1q\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 9.1 << endl;
  cout << "Teste '7.1'" << endl;
  exp = cExpressao("\"3.55\" * \"2\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 9.2 << endl;
  cout << "Teste '7.1'" << endl;
  exp = cExpressao("\"355e-2\" * \"2\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 9.3 << endl;
  cout << "Teste '7.1'" << endl;
  exp = cExpressao("\"14.2\" * \"0.5\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 9.4 << endl;
  cout << "Teste '-7.1'" << endl;
  exp = cExpressao("\"14.2\" * \"-0.5\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 10.1 << endl;
  cout << "Teste '7.1'" << endl;
  exp = cExpressao("\"3.55\" / \"0.5\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 10.2 << endl;
  cout << "Teste '7.1'" << endl;
  exp = cExpressao("\"355e-2\" / \"0.5\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 10.3 << endl;
  cout << "Teste '7.1'" << endl;
  exp = cExpressao("\"14.2\" / \"2\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 10.4 << endl;
  cout << "Teste '-7.1'" << endl;
  exp = cExpressao("\"14.2\" / \"-2\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 11.1 << endl;
  cout << "Teste ''" << endl;
  exp = cExpressao("! \"teste\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 11.2 << endl;
  cout << "Teste ''" << endl;
  exp = cExpressao("! \"true\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 11.2 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("! \"\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 12 << endl;
  cout << "Teste '12'" << endl;
  exp = cExpressao("\"1\" + \"20e-1\" * \"3\" + \"5.0\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 13 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("\"true\" || \"\" == \"true\" && \"true\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  cout << "  @Testes avaliando operações com numeros:" << endl << endl;

  try{
  cout << 1 << endl;
  cout << "Teste '12'" << endl;
  exp = cExpressao("1 + 20e-1 * 3 + 5.0", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 2 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("true || false == true && true", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 3 << endl;
  cout << "Teste ''" << endl;
  exp = cExpressao("true || false == true && false", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 4 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("false || false == true && false", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  cout << "  @Testes avaliando operações com variáveis:" << endl << endl;

  try{
  cout << 1 << endl;
  cout << "Teste '12'" << endl;
  exp = cExpressao("1 + 20e-1 * 3 + 5.0", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  return 0;
}

// */

/* * * * * END TEST cExpressao * * * * */

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