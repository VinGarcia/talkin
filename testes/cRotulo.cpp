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

  GIVEN("That it is a matcher") {
    // TODO: Elaborar esse teste.
    using namespace pMatch;
    
    cRotulo* rot = &(banco::rotulos["teste"] = cRotulo("teste"));
    
    THEN("It should match correctly") {
    
      REQUIRE_NOTHROW(rot->match("testando",0));
      CHECK(rot->getMatch().str() == "");
      
      banco::addInst("teste: \"testando\" { print('working') }");
      
      REQUIRE_NOTHROW(rot->match("testando",0));
      CHECK(rot->getMatch().str() == "testando");
    }
  }
}

/* * * * * END TEST cRotulo * * * * */
