#include "../banco.hpp"
#include <iostream>
#include "number.h"

using namespace std;

#include "catch.hpp"
#include "catch_lower.hpp"

/* * * * * START TEST cInst * * * * */

/*
 *  Teste cInst::cInst()
 */
TEST_CASE("cInst", "[cInst]") {
  using namespace pMatch;
  
  std::string expected;
  cInst inst;
  int ID;

  GIVEN("that it should build ok") {
    
    then("it should report errors correctly") {
      
      check_nothrow( cInst("teste: teste - teste => teste", ID=0) );

      check_throws( cInst(":teste: teste - teste => teste", ID=0) );

      check_throws( cInst(",teste: teste - teste => teste", ID=0) );

      check_throws( cInst("-teste: teste - teste => teste", ID=0) );

      check_nothrow( cInst("teste: teste - teste => teste", ID=0) );

      check_nothrow( cInst("teste\\: teste \\- teste => teste", ID=0) );

      check_nothrow( cInst("teste,teste,teste: teste - teste => teste", ID=0) );

      check_throws( cInst("teste,teste,: teste - teste => teste", ID=0) );

      check_throws( cInst("teste,teste,  : teste - teste => teste", ID=0) );

      check_throws( cInst("teste,,teste: teste - teste => teste", ID=0) );

      check_throws( cInst("teste,  ,teste: teste - teste => teste", ID=0) );

      check_throws( cInst(" ,teste,teste: teste - teste => teste", ID=0) );

      check_throws( cInst("   ,teste,teste: teste - teste => teste", ID=0) );

      check_nothrow( cInst("teste,teste: teste - teste => teste", ID=0) );

      check_nothrow( cInst("teste,teste:teste - teste => teste", ID=0) );

      check_nothrow( cInst("rotulo: padrao", ID=0) );

      check_nothrow( cInst("padrao => significado", ID=0) );

      check_nothrow( cInst("padrao - contexto => significado", ID=0) );

      // Rotulo omitido e um \0 após o padrão, isso gera erro:
      check_throws( cInst("padrao", ID=0) );

      check_nothrow( cInst("rotulo: padrao - contexto => significado", ID=0) );

      check_nothrow( cInst("rotulo: padrao- contexto => significado", ID=0) );

      check_nothrow( cInst("rotulo: padrao -contexto => significado", ID=0) );

      check_nothrow( cInst("rotulo: padrao - variavel.de_contexto, v1 => significado", ID=0) );

      check_nothrow( cInst("rotulo: padrao - variavel.de_contexto, v1==2 => significado", ID=0) );
      
      check_nothrow( cInst("rotulo: padrao - variavel.de_contexto; v1 => significado", ID=0) );
      
    // TODO: Corrigir o problema citado abaixo.
    cout << endl
      << " @cInst::validate, Dúvida: deve ser aceitável um `;` após a última expressão de contexto?" << endl
      << "   AINDA NAO DECIDIDO, atualmente uma exceção é lançada" << endl << endl;
      check_nothrow( cInst("rotulo: padrao - variavel.de_contexto; v1 => significado", ID=0) );
      
      check_nothrow( cInst("rotulo: padrao - contexto, v1, v2 => significado", ID=0) );
      
      check_throws( cInst("rotulo: padrao - , v1, v2 => significado", ID=0) );

      check_throws( cInst("rotulo: padrao - contexto, , v2 => significado", ID=0) );

      check_throws( cInst("rotulo: padrao - contexto, v1, => significado", ID=0) );

      check_nothrow( cInst("rotulo: padrao - contexto => sig1; sig2; sig3", ID=0) );
      
      check_nothrow( cInst("rotulo: padrao - contexto => sig1; sig2; sig3;", ID=0) );
      
      check_throws( cInst("rotulo: padrao - contexto => ; sig2; sig3", ID=0) );

      check_throws( cInst("rotulo: padrao - contexto => sig1; ; sig3", ID=0) );
      
      check_throws( cInst("rotulo: padrao - contexto => sig1; sig2; ;", ID=0) );
    }

    then("it should build with any kind of instruction") {
      
      expected = "rotulo: \"padrao\" - contexto => \"significado\";";
      check( cInst("rotulo: padrao - contexto => significado;", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - contexto => \"significado\";";
      check( cInst("       rotulo: padrao - contexto => significado;", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - contexto => \"significado\";";
      check( cInst("rotulo          : padrao - contexto => significado;", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - contexto => \"significado\";";
      check( cInst("rotulo:           padrao - contexto => significado;", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - contexto => \"significado\";";
      check( cInst("rotulo: padrao          - contexto => significado;", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - contexto => \"significado\";";
      check( cInst("rotulo: padrao -            contexto => significado;", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - contexto => \"significado\";";
      check( cInst("rotulo: padrao - contexto          => significado;", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - contexto => \"significado\";";
      check( cInst("rotulo: padrao - contexto =>          significado;", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - contexto => \"significado\";";
      check( cInst("rotulo: padrao - contexto => significado         ;", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - contexto => \"significado\";";
      check( cInst("rotulo: padrao - contexto => significado;         ", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - contexto => \"significado\";";
      check( cInst("rotulo: padrao - contexto => \"significado\";", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - contexto => \"significado\";";
      check( cInst("rotulo: \"padrao\" - contexto => \"significado\";", ID=0).str() == expected );
      
      expected = "rotulo: \" padrao\" - contexto => \"significado\";";
      check( cInst("rotulo: \" padrao\" - contexto => \"significado\";", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao \" - contexto => \"significado\";";
      check( cInst("rotulo: \"padrao \" - contexto => \"significado\";", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - contexto => \" significado\";";
      check( cInst("rotulo: \"padrao\" - contexto => \" significado\";", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - contexto => \"significado \";";
      check( cInst("rotulo: \"padrao\" - contexto => \"significado \";", ID=0).str() == expected );
      
      expected = "r1, r2, r3: \"padrao\" - contexto => \"significado\";";
      check( cInst("r1,r2,r3:\"padrao\"-contexto=>\"significado\";", ID=0).str() == expected );
      
      expected = "r1, r2, r3: \"padrao\" - contexto => \"significado\";";
      check( cInst("r1 ,r2,r3: \"padrao\" - contexto => \"significado\";", ID=0).str() == expected );
      
      expected = "r1, r2, r3: \"padrao\" - contexto => \"significado\";";
      check( cInst("r1, r2,r3: \"padrao\" - contexto => \"significado\";", ID=0).str() == expected );
      
      expected = "r1, r2, r3: \"padrao\" - contexto => \"significado\";";
      check( cInst(" r1,r2,r3: \"padrao\" - contexto => \"significado\";", ID=0).str() == expected );
      
      expected = "r1, r2, r3: \"padrao\" - contexto => \"significado\";";
      check( cInst("r1,r2,r3 : \"padrao\" - contexto => \"significado\";", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - c1, c2, c3 => \"significado\";";
      check( cInst("rotulo: \"padrao\" - c1,c2,c3 => \"significado\";", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - c1, c2, c3 => \"significado\";";
      check( cInst("rotulo: \"padrao\" - c1     ,c2,c3 => \"significado\";", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - c1, c2, c3 => \"significado\";";
      check( cInst("rotulo: \"padrao\" - c1,      c2,c3 => \"significado\";", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - c1, c2, c3 => \"significado\";";
      check( cInst("rotulo: \"padrao\" - c1,c2    ,c3 => \"significado\";", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - c1, c2, c3 => \"significado\";";
      check( cInst("rotulo: \"padrao\" - c1,c2,     c3 => \"significado\";", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - _con.texto[0] => \"significado\";";
      check( cInst("rotulo: \"padrao\" - _con.texto[0] => \"significado\";", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - contexto => #!driver: \"sig\";";
      check( cInst("rotulo: \"padrao\" - contexto => #!driver: sig;", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - contexto => #!driver: \"sig\";";
      check( cInst("rotulo: \"padrao\" - contexto => #!  driver: sig;", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - contexto => #!driver: \"sig\";";
      check( cInst("rotulo: \"padrao\" - contexto => #!driver  : sig;", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - contexto => #!driver: \"sig\";";
      check( cInst("rotulo: \"padrao\" - contexto => #!driver:sig;", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - contexto => #!driver: \"sig\";";
      check( cInst("rotulo: \"padrao\" - contexto => #!driver:  sig  ;", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - contexto => \"#!driver: sig;\";";
      check( cInst("rotulo: \"padrao\" - contexto => \"#!driver: sig;\"", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - contexto => #!driver: \"sig\";";
      check( cInst("rotulo: \"padrao\" - contexto => #!driver: \"sig\"", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - contexto => #!driver: \"sig;\";";
      check( cInst("rotulo: \"padrao\" - contexto => #!driver: \"sig;\"", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - contexto => \"sig\";";
      check( cInst("rotulo: \"padrao\" - contexto => \"sig\"", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - contexto => \"sig@nif;cado\";";
      check( cInst("rotulo: \"padrao\" - contexto => \"sig@nif;cado\"", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - contexto => \"@nif;cado\";";
      check( cInst("rotulo: \"padrao\" - contexto => \"@nif;cado\"", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - contexto => \"sig@nif;\";";
      check( cInst("rotulo: \"padrao\" - contexto => \"sig@nif;\"", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - contexto => \"sig@nif;cado\";";
      check( cInst("rotulo: \"padrao\" - contexto => sig@nif;cado", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - contexto => \"sig@nif;cado\";";
      check( cInst("rotulo: \"padrao\" - contexto => \"sig@nif  ;cado\"", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - contexto => \"sig@nif;cado\";";
      check( cInst("rotulo: \"padrao\" - contexto => \"sig@  nif;cado\"", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - contexto => \"sig@n.i.f;cado\";";
      check( cInst("rotulo: \"padrao\" - contexto => \"sig@n.i.f;cado\"", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - contexto => \"sig@n.i.f[0];cado\";";
      check( cInst("rotulo: \"padrao\" - contexto => \"sig@n.i.f[0];cado\"", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - contexto => \"sig@n.i[0].f;cado\";";
      check( cInst("rotulo: \"padrao\" - contexto => \"sig@n.i[0].f;cado\"", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - contexto => \"sig1\"; \"sig2\"; \"sig3\";";
      check( cInst("rotulo: \"padrao\" - contexto => sig1; sig2; sig3", ID=0).str() == expected );
      
      expected = "rotulo: \"padrao\" - contexto => \"@sig1;\"; \"@sig2;\"; \"@sig3;\";";
      check( cInst("rotulo: \"padrao\" - contexto => @sig1;; @sig2;; @sig3;", ID=0).str() == expected );
    }
  }
// */
}

/* * * * * END TEST cInst * * * * */
