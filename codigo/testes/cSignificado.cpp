#include "../banco.hpp"
#include <iostream>
#include "number.h"

#include "catch.hpp"
#include "catch_lower.hpp"

using namespace std;

/* * * * * START TEST cSignificado * * * * */

TEST_CASE("cSignificado", "[cSignificado]") {
  using namespace pMatch;
  std::string expected;
  uint pos;
  
  GIVEN("that it should build ok") {

    then("it should parse simple cases") {
    
      expected = "testando";
      check( cSignificado("testando", pos=0).str() == expected );
      
      expected = "'testando'";
      check( cSignificado("'testando'", pos=0).str() == expected );
      
      // expected = "\"testando\";";
      // check( cSignificado("    \"testando\";", pos=0).str() == expected );
      
      // expected = "\"testando\";";
      // check( cSignificado("\"testando\"    ;", pos=0).str() == expected );
      
      // expected = "\"testando\";";
      // check( cSignificado("\"testando\";    ", pos=0).str() == expected );
      
      // expected = "\"testando\";";
      // check( cSignificado("    testando    ;    ", pos=0).str() == expected );
    }

    then("it should parse multiple expressions") {
      const char* text;

      text = "a=1; b=2; c=a+b";
      check_nothrow(cSignificado(text, pos=0));
      check(text[pos] == '\0');

      text = " { a=1; b=2; c=a+b } ";
      check_nothrow(cSignificado(text, pos=0));
      check(text[pos-1] == '}');

      text = " { a = sin(10); b=2; c=a+b } ";
      check_nothrow(cSignificado(text, pos=0));
      check( text[pos-1] == '}');
    }
    
    then("it should recognize talkin calls") {
      check_nothrow(cSignificado("talkin('testando');", pos=0));
      check_nothrow(cSignificado("talkin(testando)    ;    ", pos=0));

      // TODO: Test talkin execution.
    }

    then("it should report errors correctly") {
      TokenMap_t map;
      cSignificado sig;
      check_nothrow(sig.exec(&map));
      check_nothrow(sig.compile("", pos=0));
      check_nothrow(sig.exec(&map));
    }
  }
}

/* * * * * END TEST cSignificado * * * * */
