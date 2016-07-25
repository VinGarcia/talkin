#include "../banco.hpp"
#include "../ambiente.hpp"
#include <iostream>
#include "number.h"

#include "catch.hpp"
#include "catch_lower.hpp"

#include "shunting-yard.h"

/* * * * * START TEST cContexto * * * * */

TEST_CASE("cContexto", "[cContexto]") {
  using std::string;
  using std::cout;
  using std::endl;

  string expected;
  cContexto con;
  uint pos;

  GIVEN("that it should build ok") {
    
    //banco::addInst("(\"[0-9][0-9]*\")p; - p = 3 => #!stdout: works!");
    //banco::execInst("3");
    
    then("it should build with any numeral expression") {
      expected = "0 == 0";
      check( expected == cContexto("0 == 0", pos=0).str() );

      expected = "10 == 10";
      check( expected == cContexto("10 == 10", pos=0).str() );

      expected = "10== 10";
      check( expected == cContexto("10== 10", pos=0).str() );

      expected = "10 ==10";
      check( expected == cContexto("10 ==10", pos=0).str() );

      expected = "10 == 10";
      check( expected == cContexto(" 10 == 10", pos=0).str() );

      expected = "10 == 10";
      check( expected == cContexto("10 == 10 ", pos=0).str() );

      expected = "\"0\" == 0";
      check( expected == cContexto("\"0\" == 0", pos=0).str() );

      expected = "var == 0";
      check( expected == cContexto("var == 0", pos=0).str() );
    }

    then("it should build with multiple concatenated expressions") {

      expected = "var1 == 0, var2 == 1";
      check( expected == cContexto("var1 == 0, var2 == 1", pos=0).str() );

    }

    then("it should report erros correctly") {
      check_throws( cContexto(", var2 == 1;", pos=0) );

      check_throws( cContexto("var1 == 0,;", pos=0) );

      check_throws( cContexto("var1 == 0,  ;", pos=0) );

      check_throws( cContexto("", pos=0) );

      check_throws( cContexto("var1 == 0;", pos=10) );

      check_throws( cContexto("var1 == 0;", pos=20) );
    }
  }

  GIVEN("that it should evaluate as expected") {
    // Criando variáveis locais:
    vars::cObject v = vars::cObject();
    v["var"] = new vars::cObject();
    v["var"] = new vars::cObject("0");
    
    check( cContexto("var['$'] == '0'", pos=0).eval(&v) == 1 );
    
    v["var"]["$"]="1";
    check( cContexto("var['$'] == '0'", pos=0).eval(&v) == 0 );
  }
}

/* * * * * END TEST cContexto * * * * */
