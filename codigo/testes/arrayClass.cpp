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

  #if NUMBER==arrayClass || NUMBER==1 || NUMBER==ALL
  {
    
    using namespace pMatch;
    arrayClass ac;
    
    cout << " * * * * * TESTE arrayClass(string) * * * * *\n\n";
    
    cout << "  @Testes com arrayClass`s:" << endl << endl;
    
    try{
    cout << 1 << endl;
    cout << "Teste \"testando\"" << endl;
    ac = arrayClass("testando");
    cout << "      " << ac.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 1.1 << endl;
    cout << "Teste \"[Tt]estando\"" << endl;
    ac = arrayClass("[Tt]estando");
    cout << "      " << ac.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 1.2 << endl;
    cout << "Teste \"[T-t]estando\"" << endl;
    ac = arrayClass("[T-t]estando");
    cout << "      " << ac.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 2 << endl;
    cout << "Teste \"  testando  \"" << endl;
    ac = arrayClass("  testando  ");
    cout << "      " << ac.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    cout << "  @Testes com blockClass`s:" << endl << endl;
    
    try{
    cout << 3 << endl;
    cout << "Teste \"(testando);\"" << endl;
    ac = arrayClass("(testando)");
    cout << "      " << ac.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 4 << endl;
    cout << "Teste \"(testando)nome;\"" << endl;
    ac = arrayClass("( testando )nome");
    cout << "      " << ac.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 5 << endl;
    cout << "Teste \"(testando)nome;\"" << endl;
    ac = arrayClass("( testando )nome;");
    cout << "      " << ac.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 6 << endl;
    cout << "Teste \"(testando)nome; \"" << endl;
    ac = arrayClass("( testando )nome ");
    cout << "      " << ac.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    cout << "  @Testes com blockClass`s e arrayClass`s:" << endl << endl;
    
    try{
    cout << 7 << endl;
    cout << "Teste \"abra o (nome de programa)p;\"" << endl;
    ac = arrayClass("abra o (nome de programa)p;");
    cout << "      " << ac.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 8 << endl;
    cout << "Teste \"(nome de verbo)v; o (nome de programa)p;\"" << endl;
    ac = arrayClass("(nome de verbo)v o (nome de programa)p");
    cout << "      " << ac.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 9 << endl;
    cout << "Teste \"(nome de verbo)v; o (nome de programa)p; agora!\"" << endl;
    ac = arrayClass("(nome de verbo)v o (nome de programa)p agora!");
    cout << "      " << ac.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 10 << endl;
    cout << "Teste \" (ab); \"" << endl;
    ac = arrayClass(" (ab) ");
    cout << "      " << ac.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    try{
    cout << 11 << endl;
    cout << "Teste \" (a); (b)c;agora!\"" << endl;
    ac = arrayClass(" (a) (b)c;agora!");
    cout << "      " << ac.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    cout << "  @Testes com caracteres incomuns" << endl << endl;
    
    try{
    cout << 12 << endl;
    cout << "Teste \"\t)(;:'\",\"" << endl;
    ac = arrayClass("\t)\\(;:'\",");
    cout << "      " << ac.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    cout << "  @Testes com erros" << endl << endl;
    
    cout << "    AINDA NAO FORAM FEITOS" << endl << endl;
    
  }
  #endif

  /*
   * Teste arrayClass::match()
   */
  #if NUMBER==match || NUMBER==2 || NUMBER==ALL
  {
    using namespace pMatch;
    arrayClass ac;
    bool bMatch;
    
    cout << " * * * * * TESTE arrayClass::match() * * * * *\n\n";
    
    cout << "  @Testes com objetos:" << endl << endl;
    
    try{
    cout << 1 << endl;
    cout << "Teste 1 \"teste\"" << endl;
    bMatch = (ac=arrayClass("teste")).match("teste",0);
    cout <<"      "<<bMatch<<" \""<< ac.getMatch().str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 2 << endl;
    cout << "Teste 1 \"teste\"" << endl;
    bMatch = (ac=arrayClass("(\"teste\");")).match("teste",0);
    cout <<"      "<<bMatch<<" \""<< ac.getMatch().str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 3 << endl;
    cout << "Teste 1 \"teste agora\"" << endl;
    bMatch = (ac=arrayClass("teste (\"agora\");")).match("teste agora",0);
    cout <<"      "<<bMatch<<" \""<< ac.getMatch().str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 4 << endl;
    cout << "Teste 1 \"agora teste\"" << endl;
    bMatch = (ac=arrayClass("(\"agora\"); teste")).match("agora teste",0);
    cout <<"      "<<bMatch<<" \""<< ac.getMatch().str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 5 << endl;
    cout << "Teste 0 \"\"" << endl;
    bMatch = (ac=arrayClass("e agora acabou")).match("e agora",0);
    cout <<"      "<<bMatch<<" \""<< ac.getMatch().str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 6 << endl;
    cout << "Teste 0 \"\"" << endl;
    bMatch = (ac=arrayClass("e (\"agora\"); acabou")).match("e agora",0);
    cout <<"      "<<bMatch<<" \""<< ac.getMatch().str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 7 << endl;
    cout << "Teste 1 \"e agora acabou\"" << endl;
    bMatch = (ac=arrayClass("e (\"agora\"); acabou")).match("e agora acabou",0);
    cout <<"      "<<bMatch<<" \""<< ac.getMatch().str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 8 << endl;
    cout << "Teste 1 \"e agora e foda\"" << endl;
    bMatch=(ac=arrayClass("e (\"agora\") e (\"foda\")")).match("e agora e foda",0);
    cout <<"      "<<bMatch<<" \""<< ac.getMatch().str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 9 << endl;
    cout << "Teste 1 \"e agora e foda\"" << endl;
    bMatch=(ac=arrayClass("(\"e\") agora (\"e\") foda")).match("e agora e foda",0);
    cout <<"      "<<bMatch<<" \""<< ac.getMatch().str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 10 << endl;
    cout << "Teste 1 \"e agora e foda\"" << endl;
    bMatch=(ac=arrayClass("(\"e \");(\"agora \");(\"e \")(\"foda\")")).match("e agora e foda",0);
    cout <<"      "<<bMatch<<" \""<< ac.getMatch().str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    cout << "CASOS COM AMBIGUIDADE:" << endl << endl;

    try{
    cout << 11 << endl;
    cout << "Teste 1 \"teste|teste\"" << endl;
    bMatch=(ac=arrayClass("(\"teste\",\"teste\")")).match("teste",0);
    cout <<"      "<<bMatch<<" \""<< ac.getMatch().str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 12 << endl;
    cout << "Teste 1 \"teste|teste\"" << endl;
    bMatch=(ac=arrayClass("(\"tes\",\"te\")(\"te\",\"ste\")")).match("teste",0);
    cout <<"      "<<bMatch<<" \""<< ac.getMatch().str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 13 << endl;
    cout << "Teste 1 \"teste agora|teste agora\"" << endl;
    bMatch=(ac=arrayClass("teste (\"ago\",\"ag\")(\"ra\",\"ora\")")).match("teste agora",0);
    cout <<"      "<<bMatch<<" \""<< ac.getMatch().str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 14 << endl;
    cout << "Teste 1 \"teste agora mesmo|teste agora mesmo\"" << endl;
    bMatch=(ac=arrayClass("teste (\"ago\",\"ag\")(\"ra\",\"ora\") mesmo")).match("teste agora mesmo",0);
    cout <<"      "<<bMatch<<" \""<< ac.getMatch().str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 15 << endl;
    cout << "Teste 1 \"teste\"" << endl;
    bMatch=(ac=arrayClass("(\"tes\",\"te\")(\"te\")")).match("teste",0);
    cout <<"      "<<bMatch<<" \""<< ac.getMatch().str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 16 << endl;
    cout << "Teste 1 \"teste\"" << endl;
    bMatch=(ac=arrayClass("(\"te\",\"tes\")(\"te\")")).match("teste",0);
    cout <<"      "<<bMatch<<" \""<< ac.getMatch().str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    cout << "CASOS COM VARIAVEIS:" << endl << endl;
    
    try{
    cout << 17 << endl;
    cout << "Teste 1 \"no_name: teste{v1: teste;}\"" << endl;
    bMatch=(ac=arrayClass("(\"teste\")v1")).match("teste agora mesmo",0);
    cout <<"      "<<bMatch<<" \""<< ac.var.str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    try{
    cout << 18 << endl;
    cout << "Teste 1 \"no_name: teste{v1: tes; v2: te;}\"" << endl;
    bMatch=(ac=arrayClass("(\"tes\")v1(\"te\")v2;")).match("teste",0);
    cout <<"      "<<bMatch<<" \""<< ac.var.str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    try{
    cout << 19 << endl;
    cout << "Teste 1 \"no_name: teste{v1: te; v2: ste;}|teste{v1: tes; v2: te;}\"" << endl;
    bMatch=(ac=arrayClass("(\"tes\",\"te\")v1(\"te\",\"ste\")v2;")).match("teste",0);
    cout <<"      "<<bMatch<<" \""<< ac.var.str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 20 << endl;
    cout << "Teste 1 \"no_name: teste{V: teste{v1: teste;};}\"" << endl;
    bMatch=(ac=arrayClass("(\"(\"teste\")v1;\")V;")).match("teste",0);
    cout <<"      "<<bMatch<<" \""<< ac.var.str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 21 << endl;
    cout << "Teste 1 \"no_name: teste{V: teste{v1: te; v2: ste;};}|teste{V: teste{v1: tes; v2: te;};}\"" << endl;
    bMatch=(ac=arrayClass("(\"(\"tes\",\"te\")v1(\"te\",\"ste\")v2\")V;")).match("teste",0);
    cout <<"      "<<bMatch<<" \""<< ac.var.str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 22 << endl;
    cout << "Teste 1 \"no_name: teste{v1: te; v2: ; v3: ste;}|teste{v1: tes; v2: ; v3: te;}|teste{v1: te; v2: s; v3: te;}\"" << endl;
    bMatch=(ac=arrayClass("(\"tes\",\"te\")v1;(\"s\",\"\")v2;(\"te\",\"ste\")v3")).match("teste",0);
    cout <<"      "<<bMatch<<" \""<< ac.var.str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    try{
    cout << 23 << endl;
    cout << "Teste 1 \"no_name: teste{V: est;}\"" << endl;
    bMatch=(ac=arrayClass("t(\"est\")V;e")).match("teste",0);
    cout <<"      "<<bMatch<<" \""<< ac.var.str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    cout << "TODO: CASOS COM OBJETOS E REPETICOES:" << endl << endl;

    try{
    cout << 24 << endl;
    cout << "Teste 1 \"no_name: 100\"" << endl;
    bMatch=(ac=arrayClass("[0-9]*")).match("100",0,true);
    cout <<"      "<<bMatch<<" \""<< ac.var.str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    try{
    cout << 25 << endl;
    cout << "Teste 1 \"no_name: 100{V: 100{[0]: 1; [1]: 0; [2]: 0;};}\"" << endl;
    bMatch=(ac=arrayClass("(\"[0-9]\")V*")).match("100",0,true);
    cout <<"      "<<bMatch<<" \""<< ac.var.str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    try{
    cout << 26 << endl;
    cout << "Teste 1 \"no_name: 100{V: 100{[0]: 1{v: 1;}; [1]: 0{v: 0;}; [2]: 0{v: 0;};};}\"" << endl;
    bMatch=(ac=arrayClass("(\"(\"[0-9]\")v\")V*")).match("100",0,true);
    cout <<"      "<<bMatch<<" \""<< ac.var.str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    try{
    cout << 27 << endl;
    cout << "Teste 1 \"no_name: 09{V: 09{[0]: 09{v1: 0; v2: 9;};};}\"" << endl;
    bMatch=(ac=arrayClass("(\"(\"[0-9]\")v1(\"[0-9]\")v2\")V*")).match("09",0,true);
    cout <<"      "<<bMatch<<" \""<< ac.var.str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
  }
  #endif
}

/* * * * * END TEST arrayClass * * * * */

