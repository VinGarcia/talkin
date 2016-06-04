#include "../padrao.hpp"
#include "../banco.hpp"
#include <iostream>
#include "number.h"

#include "catch.hpp"
#include "catch_lower.hpp"

using namespace std;

/* * * * * START TEST cRotulo * * * * */

TEST_CASE("cRotulo", "[cRotulo]") {

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
}

/* * * * * END TEST cRotulo * * * * */
