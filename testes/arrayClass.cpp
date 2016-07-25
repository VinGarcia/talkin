#include "../padrao.hpp"
#include <iostream>
#include "number.h"

#include "catch.hpp"
#include "catch_lower.hpp"

using namespace std;

/* * * * * START TEST arrayClass * * * * */

TEST_CASE("arrayClass", "[arrayClass]") {

  /*
   * Teste do  arrayClass:
   */
  std::string expected;

  GIVEN("that it should build ok") {
    
    using namespace pMatch;
    arrayClass ac;
    
    then("it should build with a given string") {
      expected = "\"testando\"";
      check( expected == arrayClass("testando").str() );

      expected = "\"[Tt]estando\"";
      check( expected == arrayClass("[Tt]estando").str() );

      expected = "\"[T-t]estando\"";
      check( expected == arrayClass("[T-t]estando").str() );

      expected = "\"  testando  \"";
      check( expected == arrayClass("  testando  ").str() );
    }

    then("it should build with nested blockClasses") {
      
      expected = "\"(testando);\"";
      check( expected == arrayClass("(testando)").str() );

      expected = "\"(testando)nome;\"";
      check( expected == arrayClass("( testando )nome").str() );

      expected = "\"(testando)nome;\"";
      check( expected == arrayClass("( testando )nome;").str() );

      expected = "\"(testando)nome; \"";
      check( expected == arrayClass("( testando )nome ").str() );
    }

    then("it should build with nested block and array Classes") {
      
      expected = "\"abra o (nome de programa)p;\"";
      check( expected == arrayClass("abra o (nome de programa)p;").str() );

      expected = "\"(nome de verbo)v; o (nome de programa)p;\"";
      check( expected == arrayClass("(nome de verbo)v o (nome de programa)p").str() );

      expected = "\"(nome de verbo)v; o (nome de programa)p; agora!\"";
      check( expected == arrayClass("(nome de verbo)v o (nome de programa)p agora!").str() );

      expected = "\" (ab); \"";
      check( expected == arrayClass(" (ab) ").str() );
      
      expected = "\" (a); (b)c;agora!\"";
      check( expected == arrayClass(" (a) (b)c;agora!").str() );
    }
    
    then("it should build even with uncommon characters") {
    
      expected = "\"\t)(;:'\",\"";
      check( expected == arrayClass("\t)\\(;:'\",").str() );
    }
    
    then("it should detect erros correctly") {
      cout << "  @Tests with errors (arrayClass)" << endl;
      cout << "    NOT DONE YET" << endl << endl;
    }
  }

  //
  // Teste arrayClass::match()
  //
  GIVEN("that it is a matcher") {
    using namespace pMatch;
    arrayClass ac;
    bool bMatch;
    
    then("it should match unambiguous blockClass patterns") {
    
      expected = "teste";
      check( 1 == (ac=arrayClass("teste")).match("teste",0) );
      check( expected == ac.getMatch().str() );

      expected = "teste";
      check( 1 == (ac=arrayClass("(\"teste\");")).match("teste",0) );
      check( expected == ac.getMatch().str() );

      expected = "teste agora";
      check( 1 == (ac=arrayClass("teste (\"agora\");")).match("teste agora",0) );
      check( expected == ac.getMatch().str() );

      expected = "agora teste";
      check( 1 == (ac=arrayClass("(\"agora\"); teste")).match("agora teste",0) );
      check( expected == ac.getMatch().str() );

      expected = "";
      check( 0 == (ac=arrayClass("e agora acabou")).match("e agora",0) );
      check( expected == ac.getMatch().str() );

      expected = "";
      check( 0 == (ac=arrayClass("e (\"agora\"); acabou")).match("e agora",0) );
      check( expected == ac.getMatch().str() );

      expected = "e agora acabou";
      check( 1 == (ac=arrayClass("e (\"agora\"); acabou")).match("e agora acabou",0) );
      check( expected == ac.getMatch().str() );

      expected = "e agora e foda";
      check( 1 ==(ac=arrayClass("e (\"agora\") e (\"foda\")")).match("e agora e foda",0) );
      check( expected == ac.getMatch().str() );

      expected = "e agora e foda";
      check( 1 ==(ac=arrayClass("(\"e\") agora (\"e\") foda")).match("e agora e foda",0) );
      check( expected == ac.getMatch().str() );

      expected = "e agora e foda";
      check( 1 ==(ac=arrayClass("(\"e \");(\"agora \");(\"e \")(\"foda\")")).match("e agora e foda",0) );
      check( expected == ac.getMatch().str() );
    }
    
    then("it should match ambiguous block patterns") {
      expected = "teste|teste";
      check( 1 ==(ac=arrayClass("(\"teste\",\"teste\")")).match("teste",0) );
      check( expected == ac.getMatch().str() );

      expected = "teste|teste";
      check( 1 ==(ac=arrayClass("(\"tes\",\"te\")(\"te\",\"ste\")")).match("teste",0) );
      check( expected == ac.getMatch().str() );

      expected = "teste agora|teste agora";
      check( 1 ==(ac=arrayClass("teste (\"ago\",\"ag\")(\"ra\",\"ora\")")).match("teste agora",0) );
      check( expected == ac.getMatch().str() );

      expected = "teste agora mesmo|teste agora mesmo";
      check( 1 ==(ac=arrayClass("teste (\"ago\",\"ag\")(\"ra\",\"ora\") mesmo")).match("teste agora mesmo",0) );
      check( expected == ac.getMatch().str() );

      expected = "teste";
      check( 1 ==(ac=arrayClass("(\"tes\",\"te\")(\"te\")")).match("teste",0) );
      check( expected == ac.getMatch().str() );

      expected = "teste";
      check( 1 ==(ac=arrayClass("(\"te\",\"tes\")(\"te\")")).match("teste",0) );
      check( expected == ac.getMatch().str() );
    }

    then("it should extract named variables from text") {
    
      expected = "no_name: teste{v1: teste;}";
      check( 1 ==(ac=arrayClass("(\"teste\")v1")).match("teste agora mesmo",0) );
      check( expected == ac.var.str() );
      
      expected = "no_name: teste{v1: tes; v2: te;}";
      check( 1 ==(ac=arrayClass("(\"tes\")v1(\"te\")v2;")).match("teste",0) );
      check( expected == ac.var.str() );
      
      expected = "no_name: teste{v1: te; v2: ste;}|teste{v1: tes; v2: te;}";
      check( 1 ==(ac=arrayClass("(\"tes\",\"te\")v1(\"te\",\"ste\")v2;")).match("teste",0) );
      check( expected == ac.var.str() );

      expected = "no_name: teste{V: teste{v1: teste;};}";
      check( 1 ==(ac=arrayClass("(\"(\"teste\")v1;\")V;")).match("teste",0) );
      check( expected == ac.var.str() );

      expected = "no_name: teste{V: teste{v1: te; v2: ste;};}|teste{V: teste{v1: tes; v2: te;};}";
      check( 1 ==(ac=arrayClass("(\"(\"tes\",\"te\")v1(\"te\",\"ste\")v2\")V;")).match("teste",0) );
      check( expected == ac.var.str() );

      expected = "no_name: teste{v1: te; v2: ; v3: ste;}|teste{v1: tes; v2: ; v3: te;}|teste{v1: te; v2: s; v3: te;}";
      check( 1 ==(ac=arrayClass("(\"tes\",\"te\")v1;(\"s\",\"\")v2;(\"te\",\"ste\")v3")).match("teste",0) );
      check( expected == ac.var.str() );
      
      expected = "no_name: teste{V: est;}";
      check( 1 ==(ac=arrayClass("t(\"est\")V;e")).match("teste",0) );
      check( expected == ac.var.str() );
    }
    
    then("it should match cases with kleene operator") {

      cout << "  @Cases with objects and repetitions (arrayClass)" << endl;
      cout << "    INCOMPLETE" << endl << endl;

      expected = "no_name: 100";
      check( 1 ==(ac=arrayClass("[0-9]*")).match("100",0,true) );
      check( expected == ac.var.str() );
      
      expected = "no_name: 100{V: 100{[0]: 1; [1]: 0; [2]: 0;};}";
      check( 1 ==(ac=arrayClass("(\"[0-9]\")V*")).match("100",0,true) );
      check( expected == ac.var.str() );
      
      expected = "no_name: 100{V: 100{[0]: 1{v: 1;}; [1]: 0{v: 0;}; [2]: 0{v: 0;};};}";
      check( 1 ==(ac=arrayClass("(\"(\"[0-9]\")v\")V*")).match("100",0,true) );
      check( expected == ac.var.str() );
      
      expected = "no_name: 09{V: 09{[0]: 09{v1: 0; v2: 9;};};}";
      check( 1 ==(ac=arrayClass("(\"(\"[0-9]\")v1(\"[0-9]\")v2\")V*")).match("09",0,true) );
      check( expected == ac.var.str() );

    }
  }
}

/* * * * * END TEST arrayClass * * * * */

