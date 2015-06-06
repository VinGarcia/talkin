#include "../banco.hpp"
#include "../ambiente.hpp"
#include <iostream>
#include "number.h"

/* * * * * START TEST cContexto * * * * */

int main(int argc, char* argv[])
{
  using namespace std;
  using namespace pMatch;
  cContexto con;
  int pos;

#if NUMBER==cContexto || NUMBER==1 || NUMBER==ALL
{
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
}
#endif

#if NUMBER==eval || NUMBER==2 || NUMBER==ALL
{
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
}
#endif
  
  return 0;
}

/* * * * * END TEST cContexto * * * * */