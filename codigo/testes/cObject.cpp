#include "../padrao.hpp"
#include "../vars.hpp"
#include <iostream>
#include "number.h"

#include "catch.hpp"
#include "catch_lower.hpp"

using namespace std;

/* * * * * START TEST cObject * * * * */

TEST_CASE("cObject", "[cObject]") {
  vars::cObject* dad;
  vars::cObject* var;
  std::string expected;

  using vars::cObject;

  GIVEN("that it should build ok") {

    then("it should build with string arguments:") {
      expected = "{ \"$\": \"valor\" }";
      check( cObject("valor").str() == expected );

      expected = "{ \"$\": \"valor\" }";
      check( cObject(string("valor")).str() == expected );

      expected = "{ \"$\": \"\" }";
      check( cObject("").str() == expected );

      expected = "{ \"$\": \"\" }";
      check( cObject(string("")).str() == expected );

      expected = "{}";
      check( cObject().str() == expected );
    }
  }

  GIVEN("that it should build from a pMatch::cVars object") {

    std::string expected;

    then("it should copy it precisely") {

      pMatch::arrayClass array;
      pMatch::arrayClass array2;
      pMatch::cVar* pmv;

      // Depends on arrayClass constructor and arrayClass operator=()
      require_nothrow(
        array = pMatch::arrayClass("(\"tes\",\"te\")a(\"s\",\"\")b(\"te\",\"ste\")c")
      );
      require_nothrow(
        array2 = pMatch::arrayClass("(\"(\"teste\")b\", \"tes(\"te\")c\")a")
      );

      // Save array.var address:
      pmv = &(array.var);

      // cout << "pMatch::var: " << endl;
      // cout << pmv->str() << endl;

      array.match("tete",0);
      expected = "{ \"$\": \"tete\", \"a\": { \"$\": \"te\" }, \"b\": {}, \"c\": { \"$\": \"te\" } }";
      check( cObject(*pmv).str() == expected );
      
      array.match("tesste",0);
      expected = "{ \"$\": \"tesste\", \"a\": { \"$\": \"tes\" }, \"b\": {}, \"c\": { \"$\": \"ste\" } }";
      check( cObject(*pmv).str() == expected );

      array2.match("teste",0);
      expected = "{ \"$\": \"teste\", \"a\": { \"$\": \"teste\", \"c\": { \"$\": \"te\" } } }";
      check( cObject(array2.var).str() == expected );
    }
  }
}

/* * * * * END TEST cObject * * * * */










