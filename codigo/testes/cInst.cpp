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
  
  cInst inst;
  int ID;

  #if NUMBER==cInst || NUMBER==1 || NUMBER==ALL
  {
    
    cout << " * * * * * TESTE cInst(string) * * * * *\n\n";
    
    cout << "  @Testes com a validação:" << endl << endl;
    
    try{
    cout << 1 << endl;
    cout << "Teste inicializado" << endl;
    cInst("teste: teste - teste => teste", ID=0);
    cout << "      " << "inicializado" << endl << endl;
    }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

    try{
    cout << 1.1 << endl;
    cout << "Teste erro lançado!" << endl;
    cInst(":teste: teste - teste => teste", ID=0);
    cout << "      " << "inicializado" << endl << endl;
    }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

    try{
    cout << 1.2 << endl;
    cout << "Teste erro lançado!" << endl;
    cInst(",teste: teste - teste => teste", ID=0);
    cout << "      " << "inicializado" << endl << endl;
    }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

    try{
    cout << 1.3 << endl;
    cout << "Teste erro lançado!" << endl;
    cInst("-teste: teste - teste => teste", ID=0);
    cout << "      " << "inicializado" << endl << endl;
    }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

    try{
    cout << 2 << endl;
    cout << "Teste inicializado" << endl;
    cInst("teste: teste - teste => teste", ID=0);
    cout << "      " << "inicializado" << endl << endl;
    }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

    try{
    cout << 2.1 << endl;
    cout << "Teste inicializado" << endl;
    cInst("teste\\: teste \\- teste => teste", ID=0);
    cout << "      " << "inicializado" << endl << endl;
    }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

    try{
    cout << 3 << endl;
    cout << "Teste inicializado" << endl;
    cInst("teste,teste,teste: teste - teste => teste", ID=0);
    cout << "      " << "inicializado" << endl << endl;
    }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

    try{
    cout << 3.1 << endl;
    cout << "Teste erro lançado!" << endl;
    cInst("teste,teste,: teste - teste => teste", ID=0);
    cout << "      " << "inicializado" << endl << endl;
    }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

    try{
    cout << 3.1 << endl;
    cout << "Teste erro lançado!" << endl;
    cInst("teste,teste,  : teste - teste => teste", ID=0);
    cout << "      " << "inicializado" << endl << endl;
    }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

    try{
    cout << 3.2 << endl;
    cout << "Teste erro lançado!" << endl;
    cInst("teste,,teste: teste - teste => teste", ID=0);
    cout << "      " << "inicializado" << endl << endl;
    }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

    try{
    cout << 3.2 << endl;
    cout << "Teste erro lançado!" << endl;
    cInst("teste,  ,teste: teste - teste => teste", ID=0);
    cout << "      " << "inicializado" << endl << endl;
    }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

    try{
    cout << 3.3 << endl;
    cout << "Teste erro lançado!" << endl;
    cInst(" ,teste,teste: teste - teste => teste", ID=0);
    cout << "      " << "inicializado" << endl << endl;
    }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

    try{
    cout << 3.3 << endl;
    cout << "Teste erro lançado!" << endl;
    cInst("   ,teste,teste: teste - teste => teste", ID=0);
    cout << "      " << "inicializado" << endl << endl;
    }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

    try{
    cout << 4 << endl;
    cout << "Teste inicializado" << endl;
    cInst("teste,teste: teste - teste => teste", ID=0);
    cout << "      " << "inicializado" << endl << endl;
    }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

    try{
    cout << 4.1 << endl;
    cout << "Teste inicializado" << endl;
    cInst("teste,teste:teste - teste => teste", ID=0);
    cout << "      " << "inicializado" << endl << endl;
    }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

    try{
    cout << 5 << endl;
    cout << "Teste inicializado" << endl;
    cInst("rotulo: padrao", ID=0);
    cout << "      " << "inicializado" << endl << endl;
    }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

    try{
    cout << 5 << endl;
    cout << "Teste inicializado" << endl;
    cInst("padrao => significado", ID=0);
    cout << "      " << "inicializado" << endl << endl;
    }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

    try{
    cout << 5 << endl;
    cout << "Teste inicializado" << endl;
    cInst("padrao - contexto => significado", ID=0);
    cout << "      " << "inicializado" << endl << endl;
    }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

    // Rotulo omitido e um \0 após o padrão, isso gera erro:
    try{
    cout << 5.1 << endl;
    cout << "Teste erro lançado!" << endl;
    cInst("padrao", ID=0);
    cout << "      " << "inicializado" << endl << endl;
    }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

    try{
    cout << 6 << endl;
    cout << "Teste inicializado" << endl;
    cInst("rotulo: padrao - contexto => significado", ID=0);
    cout << "      " << "inicializado" << endl << endl;
    }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

    try{
    cout << 6.1 << endl;
    cout << "Teste inicializado" << endl;
    cInst("rotulo: padrao- contexto => significado", ID=0);
    cout << "      " << "inicializado" << endl << endl;
    }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

    try{
    cout << 6.2 << endl;
    cout << "Teste inicializado" << endl;
    cInst("rotulo: padrao -contexto => significado", ID=0);
    cout << "      " << "inicializado" << endl << endl;
    }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

    try{
    cout << 7 << endl;
    cout << "Teste inicializado" << endl;
    cInst("rotulo: padrao - variavel.de_contexto, v1 => significado", ID=0);
    cout << "      " << "inicializado" << endl << endl;
    }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

  //xxx
    try{
    cout << 7.1 << endl;
    cout << "Teste inicializado" << endl;
    cInst("rotulo: padrao - variavel.de_contexto, v1=2 => significado", ID=0);
    cout << "      " << "inicializado" << endl << endl;
    }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }
    
    try{
    cout << 7.2 << endl;
    cout << "Teste inicializado" << endl;
    cInst("rotulo: padrao - variavel.de_contexto; v1 => significado", ID=0);
    cout << "      " << "inicializado" << endl << endl;
    }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }
    
  cout
    << " O teste abaixo realça um problema: " << endl
    << " 'rot: pad - v1, v2' é correto, mas 'rot: pad - v1, v2,' não é."<< endl
    << " 'rot: pad - v1; v2; é correto, e 'rot: pad - v1; v2' também é."<< endl
    << " Isso deverá ser corrigido no futuro." << endl;
  // TODO: Corrigir o problema citado acima.
    try{
    cout << 7.3 << endl;
    cout << "Teste inicializado" << endl;
    cInst("rotulo: padrao - variavel.de_contexto; v1; => significado", ID=0);
    cout << "      " << "inicializado" << endl << endl;
    }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }
    
    try{
    cout << 8 << endl;
    cout << "Teste inicializado" << endl;
    cInst("rotulo: padrao - contexto, v1, v2 => significado", ID=0);
    cout << "      " << "inicializado" << endl << endl;
    }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }
    
    try{
    cout << 8.1 << endl;
    cout << "Teste erro lançado!" << endl;
    cInst("rotulo: padrao - , v1, v2 => significado", ID=0);
    cout << "      " << "inicializado" << endl << endl;
    }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

    try{
    cout << 8.2 << endl;
    cout << "Teste erro lançado!" << endl;
    cInst("rotulo: padrao - contexto, , v2 => significado", ID=0);
    cout << "      " << "inicializado" << endl << endl;
    }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

    try{
    cout << 8.3 << endl;
    cout << "Teste erro lançado!" << endl;
    cInst("rotulo: padrao - contexto, v1, => significado", ID=0);
    cout << "      " << "inicializado" << endl << endl;
    }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

    try{
    cout << 9 << endl;
    cout << "Teste inicializado" << endl;
    cInst("rotulo: padrao - contexto => sig1; sig2; sig3", ID=0);
    cout << "      " << "inicializado" << endl << endl;
    }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }
    
    try{
    cout << 9 << endl;
    cout << "Teste inicializado" << endl;
    cInst("rotulo: padrao - contexto => sig1; sig2; sig3;", ID=0);
    cout << "      " << "inicializado" << endl << endl;
    }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }
    
    try{
    cout << 9.1 << endl;
    cout << "Teste erro lançado!" << endl;
    cInst("rotulo: padrao - contexto => ; sig2; sig3", ID=0);
    cout << "      " << "inicializado" << endl << endl;
    }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

    try{
    cout << 9.2 << endl;
    cout << "Teste erro lançado!" << endl;
    cInst("rotulo: padrao - contexto => sig1; ; sig3", ID=0);
    cout << "      " << "inicializado" << endl << endl;
    }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }
    
    try{
    cout << 9.3 << endl;
    cout << "Teste erro lançado!" << endl;
    cInst("rotulo: padrao - contexto => sig1; sig2; ;", ID=0);
    cout << "      " << "inicializado" << endl << endl;
    }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }
  }
  #endif

  #if NUMBER==str || NUMBER==2 || NUMBER==ALL
  {
    cout << "TESTE DE CONSTRUCAO e IMPRESSAO:" << endl << endl;
    
    try{
    cout << 1 << endl;
    cout << "Teste rotulo: \"padrao\" - contexto => \"significado\";" << endl;
    inst = cInst("rotulo: padrao - contexto => significado;", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 2 << endl;
    cout << "Teste rotulo: \"padrao\" - contexto => \"significado\";" << endl;
    inst = cInst("       rotulo: padrao - contexto => significado;", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 3 << endl;
    cout << "Teste rotulo: \"padrao\" - contexto => \"significado\";" << endl;
    inst = cInst("rotulo          : padrao - contexto => significado;", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 4 << endl;
    cout << "Teste rotulo: \"padrao\" - contexto => \"significado\";" << endl;
    inst = cInst("rotulo:           padrao - contexto => significado;", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 5 << endl;
    cout << "Teste rotulo: \"padrao\" - contexto => \"significado\";" << endl;
    inst = cInst("rotulo: padrao          - contexto => significado;", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 6 << endl;
    cout << "Teste rotulo: \"padrao\" - contexto => \"significado\";" << endl;
    inst = cInst("rotulo: padrao -            contexto => significado;", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 7 << endl;
    cout << "Teste rotulo: \"padrao\" - contexto => \"significado\";" << endl;
    inst = cInst("rotulo: padrao - contexto          => significado;", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 8 << endl;
    cout << "Teste rotulo: \"padrao\" - contexto => \"significado\";" << endl;
    inst = cInst("rotulo: padrao - contexto =>          significado;", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 9 << endl;
    cout << "Teste rotulo: \"padrao\" - contexto => \"significado\";" << endl;
    inst = cInst("rotulo: padrao - contexto => significado         ;", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 10 << endl;
    cout << "Teste rotulo: \"padrao\" - contexto => \"significado\";" << endl;
    inst = cInst("rotulo: padrao - contexto => significado;         ", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 11 << endl;
    cout << "Teste rotulo: \"padrao\" - contexto => \"significado\";" << endl;
    inst = cInst("rotulo: padrao - contexto => \"significado\";", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 12 << endl;
    cout << "Teste rotulo: \"padrao\" - contexto => \"significado\";" << endl;
    inst = cInst("rotulo: \"padrao\" - contexto => \"significado\";", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 13 << endl;
    cout << "Teste rotulo: \" padrao\" - contexto => \"significado\";" << endl;
    inst = cInst("rotulo: \" padrao\" - contexto => \"significado\";", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 14 << endl;
    cout << "Teste rotulo: \"padrao \" - contexto => \"significado\";" << endl;
    inst = cInst("rotulo: \"padrao \" - contexto => \"significado\";", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 15 << endl;
    cout << "Teste rotulo: \"padrao\" - contexto => \" significado\";" << endl;
    inst = cInst("rotulo: \"padrao\" - contexto => \" significado\";", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 16 << endl;
    cout << "Teste rotulo: \"padrao\" - contexto => \"significado \";" << endl;
    inst = cInst("rotulo: \"padrao\" - contexto => \"significado \";", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 17 << endl;
    cout << "Teste r1, r2, r3: \"padrao\" - contexto => \"significado\";"<< endl;
    inst = cInst("r1,r2,r3:\"padrao\"-contexto=>\"significado\";", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 17.1 << endl;
    cout << "Teste r1, r2, r3: \"padrao\" - contexto => \"significado\";"<< endl;
    inst = cInst("r1 ,r2,r3: \"padrao\" - contexto => \"significado\";", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 17.2 << endl;
    cout << "Teste r1, r2, r3: \"padrao\" - contexto => \"significado\";"<< endl;
    inst = cInst("r1, r2,r3: \"padrao\" - contexto => \"significado\";", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 17.3 << endl;
    cout << "Teste r1, r2, r3: \"padrao\" - contexto => \"significado\";"<< endl;
    inst = cInst(" r1,r2,r3: \"padrao\" - contexto => \"significado\";", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 17.4 << endl;
    cout << "Teste r1, r2, r3: \"padrao\" - contexto => \"significado\";"<< endl;
    inst = cInst("r1,r2,r3 : \"padrao\" - contexto => \"significado\";", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 18 << endl;
    cout << "Teste rotulo: \"padrao\" - c1, c2, c3 => \"significado\";" << endl;
    inst = cInst("rotulo: \"padrao\" - c1,c2,c3 => \"significado\";", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 18.1 << endl;
    cout << "Teste rotulo: \"padrao\" - c1, c2, c3 => \"significado\";" << endl;
    inst = cInst("rotulo: \"padrao\" - c1     ,c2,c3 => \"significado\";", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 18.2 << endl;
    cout << "Teste rotulo: \"padrao\" - c1, c2, c3 => \"significado\";" << endl;
    inst = cInst("rotulo: \"padrao\" - c1,      c2,c3 => \"significado\";", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 18.3 << endl;
    cout << "Teste rotulo: \"padrao\" - c1, c2, c3 => \"significado\";" << endl;
    inst = cInst("rotulo: \"padrao\" - c1,c2    ,c3 => \"significado\";", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 18.4 << endl;
    cout << "Teste rotulo: \"padrao\" - c1, c2, c3 => \"significado\";" << endl;
    inst = cInst("rotulo: \"padrao\" - c1,c2,     c3 => \"significado\";", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 18.5 << endl;
    cout <<"Teste rotulo: \"padrao\" - _con.texto[0] => \"significado\";" << endl;
    inst = cInst("rotulo: \"padrao\" - _con.texto[0] => \"significado\";", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 19 << endl;
    cout << "Teste rotulo: \"padrao\" - contexto => #!driver: \"sig\";" << endl;
    inst = cInst("rotulo: \"padrao\" - contexto => #!driver: sig;", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 19.1 << endl;
    cout << "Teste rotulo: \"padrao\" - contexto => #!driver: \"sig\";" << endl;
    inst = cInst("rotulo: \"padrao\" - contexto => #!  driver: sig;", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 19.2 << endl;
    cout << "Teste rotulo: \"padrao\" - contexto => #!driver: \"sig\";" << endl;
    inst = cInst("rotulo: \"padrao\" - contexto => #!driver  : sig;", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 19.3 << endl;
    cout << "Teste rotulo: \"padrao\" - contexto => #!driver: \"sig\";" << endl;
    inst = cInst("rotulo: \"padrao\" - contexto => #!driver:sig;", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 19.4 << endl;
    cout << "Teste rotulo: \"padrao\" - contexto => #!driver: \"sig\";" << endl;
    inst = cInst("rotulo: \"padrao\" - contexto => #!driver:  sig  ;", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 20 << endl;
    cout << "Teste rotulo: \"padrao\" - contexto => \"#!driver: sig;\";" << endl;
    inst = cInst("rotulo: \"padrao\" - contexto => \"#!driver: sig;\"", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 21 << endl;
    cout << "Teste rotulo: \"padrao\" - contexto => #!driver: \"sig\";" << endl;
    inst = cInst("rotulo: \"padrao\" - contexto => #!driver: \"sig\"", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 22 << endl;
    cout << "Teste rotulo: \"padrao\" - contexto => #!driver: \"sig;\";" << endl;
    inst = cInst("rotulo: \"padrao\" - contexto => #!driver: \"sig;\"", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 23 << endl;
    cout << "Teste rotulo: \"padrao\" - contexto => \"sig\";" << endl;
    inst = cInst("rotulo: \"padrao\" - contexto => \"sig\"", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 24 << endl;
    cout << "Teste rotulo: \"padrao\" - contexto => \"sig@nif;cado\";" << endl;
    inst = cInst("rotulo: \"padrao\" - contexto => \"sig@nif;cado\"", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 24.1 << endl;
    cout << "Teste rotulo: \"padrao\" - contexto => \"@nif;cado\";" << endl;
    inst = cInst("rotulo: \"padrao\" - contexto => \"@nif;cado\"", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 24.2 << endl;
    cout << "Teste rotulo: \"padrao\" - contexto => \"sig@nif;\";" << endl;
    inst = cInst("rotulo: \"padrao\" - contexto => \"sig@nif;\"", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 24.3 << endl;
    cout << "Teste rotulo: \"padrao\" - contexto => \"sig@nif;cado\";" << endl;
    inst = cInst("rotulo: \"padrao\" - contexto => sig@nif;cado", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 24.4 << endl;
    cout << "Teste rotulo: \"padrao\" - contexto => \"sig@nif;cado\";" << endl;
    inst = cInst("rotulo: \"padrao\" - contexto => \"sig@nif  ;cado\"", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 24.5 << endl;
    cout << "Teste rotulo: \"padrao\" - contexto => \"sig@nif;cado\";" << endl;
    inst = cInst("rotulo: \"padrao\" - contexto => \"sig@  nif;cado\"", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 24.6 << endl;
    cout << "Teste rotulo: \"padrao\" - contexto => \"sig@n.i.f;cado\";" << endl;
    inst = cInst("rotulo: \"padrao\" - contexto => \"sig@n.i.f;cado\"", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 24.7 << endl;
    cout << "Teste rotulo: \"padrao\" - contexto => \"sig@n.i.f[0];cado\";"<<endl;
    inst = cInst("rotulo: \"padrao\" - contexto => \"sig@n.i.f[0];cado\"", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 24.8 << endl;
    cout << "Teste rotulo: \"padrao\" - contexto => \"sig@n.i[0].f;cado\";"<<endl;
    inst = cInst("rotulo: \"padrao\" - contexto => \"sig@n.i[0].f;cado\"", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 25 << endl;
    cout << "Teste rotulo: \"padrao\" - contexto => \"sig1\"; \"sig2\"; \"sig3\";"<<endl;
    inst = cInst("rotulo: \"padrao\" - contexto => sig1; sig2; sig3", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
    
    try{
    cout << 25 << endl;
    cout << "Teste rotulo: \"padrao\" - contexto => \"@sig1;\"; \"@sig2;\"; \"@sig3;\";"<<endl;
    inst = cInst("rotulo: \"padrao\" - contexto => @sig1;; @sig2;; @sig3;", ID=0);
    cout << "      " << inst.str() << endl << endl;
    }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  }
  #endif
}

/* * * * * END TEST cInst * * * * */
