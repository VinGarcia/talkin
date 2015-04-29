#include "../padrao.hpp"
#include "../banco.hpp"
#include <iostream>
#include "number.h"

using namespace std;

/* * * * * START TEST arrayClass * * * * */

int main(int argc, char* argv[])
{

/*
 * Teste do objectClass:
 */

#if NUMBER==objectClass || NUMBER==1 || NUMBER==ALL
{

  // Teste do objectClass:
  using namespace pMatch;
  
  objectClass oc;
  int pos;
  
  cout << " * * * * * TESTE objectClass() * * * * *\n\n";
  
  cout << "  @Testes com objectClass(string):" << endl << endl;
  
  try{
  cout << 1 << endl;
  cout << "Teste \"verbo\"" << endl;
  oc = objectClass("verbo");
  cout << "      \"" << oc.str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 2 << endl;
  cout << "Teste \"verbo dois\"" << endl;
  oc = objectClass("verbo      dois");
  cout << "      \"" << oc.str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 3 << endl;
  cout << "Teste \"verbo tres\"" << endl;
  oc = objectClass("    verbo      tres");
  cout << "      \"" << oc.str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 4 << endl;
  cout << "Teste \"verbo quatro\"" << endl;
  oc = objectClass("    verbo      quatro    ");
  cout << "      \"" << oc.str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  cout << "  @Testes com objectClass(string, pos):" << endl << endl;
  
  try{
  cout << 5 << endl;
  cout << "Teste \"verbo\"" << endl;
  oc = objectClass("verbo", pos=0);
  cout << "      \"" << oc.str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 6 << endl;
  cout << "Teste \"verbo\"" << endl;
  oc = objectClass("averbo", pos=1);
  cout << "      \"" << oc.str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 7 << endl;
  cout << "Teste \"verbo\"" << endl;
  oc = objectClass("  verbo  ", pos=1);
  cout << "      \"" << oc.str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 8 << endl;
  cout << "Teste \"verbo\"" << endl;
  oc = objectClass("verbo, verbula", pos=0);
  cout << "      \"" << oc.str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 9 << endl;
  cout << "Teste \"verbo\"" << endl;
  oc = objectClass("Verbo, verbo  ) amacada", pos);
  cout << "      \"" << oc.str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  cout << "  @Testes de validação do objectClass():" << endl << endl;
  
  try{
  cout << 10 << endl;
  cout << "Teste throwed" << endl;
  oc = objectClass("Verb,o");
  cout << "      \"" << oc.str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << "      " << "throwed" << endl << endl; }
  
  try{
  cout << 11 << endl;
  cout << "Teste throwed" << endl;
  oc = objectClass("Verb:o");
  cout << "      \"" << oc.str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << "      " << "throwed" << endl << endl; }
  
  try{
  cout << 12 << endl;
  cout << "Teste throwed" << endl;
  oc = objectClass("Verb(o");
  cout << "      \"" << oc.str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << "      " << "throwed" << endl << endl; }
  
  try{
  cout << 13 << endl;
  cout << "Teste throwed" << endl;
  oc = objectClass("Verb)o");
  cout << "      \"" << oc.str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << "      " << "throwed" << endl << endl; }
  
  try{
  cout << 14 << endl;
  cout << "Teste not throwed" << endl;
  oc = objectClass("Verb.!@#$%^&*-'\"\t\n o");
  cout << "      " << "not throwed" << endl << endl;
  }catch(const char* c){ cout << "      " << "throwed" << endl << endl; }
}
#endif
  
/*
 * Testando blockClass::match()
 */

#if NUMBER==match || NUMBER==2 || NUMBER==ALL
{
  using namespace pMatch;
  
  objectClass oc = objectClass("teste");
  
  cout << " * * * * * TESTE objectClass::match() * * * * *\n\n";
  
  cout << "  @Testes com objectClass(string):" << endl << endl;
  
  try{
  cout << 1 << endl;
  cout << "Teste \"\"" << endl;
  oc.match("testando",0);
  cout << "      \"" << oc.getMatch().str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl << endl; }
  
  banco::addInst("teste: testando => #!stdout: working");
  
  try{
  cout << 2 << endl;
  cout << "Teste \"testando\"" << endl;
  oc.match("testando",0);
  cout << "      \"" << oc.getMatch().str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl << endl; }
  
  try{
  cout << 2.1 << endl;
  cout << "Teste \"testando\"" << endl;
  oc.match("testestando",3);
  cout << "      \"" << oc.getMatch().str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl << endl; }
  
  try{
  cout << 2.2 << endl;
  cout << "Teste \"testando\"" << endl;
  oc.match("testestandoagora",3);
  cout << "      \"" << oc.getMatch().str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl << endl; }
  
  banco::addInst("teste: test => #!stdout: working");
  
  try{
  cout << 3 << endl;
  cout << "Teste \"testando\"" << endl;
  oc.match("testestandoagora",3);
  cout << "      \"" << oc.getMatch().str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl << endl; }
}
#endif
  
  return 0;
}

/* * * * * END TEST objectClass * * * * */
