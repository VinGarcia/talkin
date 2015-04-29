#include "../padrao.hpp"
#include "../banco.hpp"
#include <iostream>
#include "number.h"

using namespace std;

/* * * * * START TEST arrayClass * * * * */

int main(int argc, char* argv[])
{

/*
 * Teste do cRotulo:
 */

#if NUMBER==objectClass || NUMBER==1 || NUMBER==ALL
{
  // TODO: Elaborar esse teste.
  using namespace pMatch;
  
  cRotulo* rot = &(banco::rotulos["teste"] = cRotulo("teste"));
  
  cout << " * * * * * TESTE cRotulo.match() * * * * *\n\n";
  
  cout << "  @Testes com cRotulo(string):" << endl << endl;
  
  try{
  cout << 1 << endl;
  cout << "Teste \"\"" << endl;
  rot->match("testando",0);
  cout << "      \"" << rot->getMatch().str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl << endl; }
  
  banco::addInst("teste: testando => #!stdout: working");
  
  try{
  cout << 2 << endl;
  cout << "Teste \"testando\"" << endl;
  rot->match("testando",0);
  cout << "      \"" << rot->getMatch().str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl << endl; }
}
#endif
  
  return 0;
}

/* * * * * END TEST cRotulo * * * * */
