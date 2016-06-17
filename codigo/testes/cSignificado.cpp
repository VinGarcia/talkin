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
  int pos;
  
  GIVEN("that it should build ok") {

    then("it should parse string literals") {
    
      expected = "\"testando\";";
      check( cSignificado("testando", pos=0).str() == expected );
      
      expected = "\"testando\";";
      check( cSignificado("testando;", pos=0).str() == expected );
      
      expected = "\"testando\";";
      check( cSignificado("\"testando\";", pos=0).str() == expected );
      
      expected = "\"testando\";";
      check( cSignificado("    \"testando\";", pos=0).str() == expected );
      
      expected = "\"testando\";";
      check( cSignificado("\"testando\"    ;", pos=0).str() == expected );
      
      expected = "\"testando\";";
      check( cSignificado("\"testando\";    ", pos=0).str() == expected );
      
      expected = "\"testando\";";
      check( cSignificado("    testando    ;    ", pos=0).str() == expected );
    }
    
    then("it should recognize driver calls") {
    
      expected = "#!talkin: \"testando\";";
      check( cSignificado("#!talkin: testando;", pos=0).str() == expected );
      
      expected = "#!talkin: \"testando\";";
      check( cSignificado("#!talkin:testando    ;    ", pos=0).str() == expected );
      
      expected = "#!talkin: \"testando\";";
      check( cSignificado("#!  talkin  : testando;", pos=0).str() == expected );
      
      expected = "#!talkin: \"testando\";";
      check( cSignificado("      #!talkin: testando;", pos=0).str() == expected );
    }
    
    then("it should parse sequential cSignificados from a string") {

      expected = "#!talkin: \"testando\";";
      cSignificado("#!talkin: teste;", pos=0);
      check( cSignificado("#!talkin: teste; #!talkin: testando;", pos).str() == expected );
      
      expected = "\"teste\";";
      check( cSignificado("#!talkin: teste; #!talkin: testando; teste", pos).str() == expected );
    }
  }
}

TEST_CASE("cSignificado TODO", "[cSignificado][!mayfail]") {
  
  int pos;
  std::string expected;

  GIVEN("that it should report erros when building") {
    then("it should check for valid driver names") {
      cout
      << "Este teste deveria lançar um erro." << endl
      << "por enquanto o comportamento é aceitável porque a validade()" << endl
      << "Ainda não está implementada, porém irei deixa-lo aqui como um lembrete"
      << endl;

      expected = "#!talkin: \"testando\";";
      check_throws( cSignificado("#!  tal  kin  : testando;", pos=0) );
    }
  }
}
    

/* * * * * END TEST cSignificado * * * * */
