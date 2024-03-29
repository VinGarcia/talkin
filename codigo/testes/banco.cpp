#include "../padrao.hpp"
#include "../banco.hpp"
#include "../ambiente.hpp"
#include <iostream>
#include "number.h"

#include "catch.hpp"
#include "catch_lower.hpp"

using namespace std;
using namespace ambiente;

// Drivers do talkin:
//std::map<std::string, driver*> ambiente::drivers;

/* * * * * START TEST banco * * * * */

#define PRINT_LIST() for(auto& a : banco::strList()) cout << "      " << a << endl

void COMPARE(const banco::StrList_t& a, const banco::StrList_t& b) {

  REQUIRE(a.size() == b.size());

  banco::StrList_t::const_iterator ita = a.begin();
  banco::StrList_t::const_iterator itb = b.begin();

  while (ita != a.end() && itb != b.end()) {
    CHECK(*ita == *itb);
    ++ita;
    ++itb;
  }
}

TEST_CASE("banco", "[banco]") {

  /*
   * Teste do banco:
   */

  GIVEN("That it should manage instructions") {

    banco::StrList_t expected_list;
    
    THEN("It should add instructions correctly") {
      
      expected_list.push_back("inst[1][0]: «rot: \"pad\" - contx { talkin('sig') }»");
      REQUIRE_NOTHROW(banco::addInst("rot: \"pad\" - contx { talkin('sig') }"));
      COMPARE(banco::strList(), expected_list);
      
      expected_list.push_back("inst[1][1]: «rot2: \"pad2\" - contx2 { talkin('sig2') }»");
      REQUIRE_NOTHROW(banco::addInst("rot2: \"pad2\" - contx2 { talkin('sig2') }"));
      COMPARE(banco::strList(), expected_list);
    }
    /*
    cout << "  @Teste com o remInst()" << endl << endl;
    
    try{
    cout << 3 << endl;
    cout << "Teste inst[1][0]: «rot: \"pad\" - contx => \"sig\";»" << endl;
    banco::remInst(1);
    PRINT_LIST(); cout << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    cout << "  @Teste com o execInst()" << endl << endl;
    
    // Adicionando o driver stdout e talkin para a lista de drivers:
    drivers["stdout"] = new driver_stdout();
    drivers["talkin"] = new driver_talkin();
    
    try{
    cout << 4 << endl;
    cout << "Teste Hello World!" << endl;
    banco::addInst("\"[Oo]la!\" { print('Hello World!') }");
    cout << "      "; banco::execInst("Ola!"); cout << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    try{
    cout << 4.1 << endl;
    cout << "Teste Hello World!" << endl;
    banco::addInst("\"start!\" { talkin('ola!') }");
    cout << "      "; banco::execInst("start!"); cout << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    cout << "  @Teste utilizando o valor de uma variável" << endl << endl;
    
    try{
    cout << 5 << endl;
    cout << "Teste hello world!" << endl;
    banco::addInst("\"(\"[Hh]ello [wW]orld!*\")v;\" { print(v) }");
    cout << "      "; banco::execInst("hello world!"); cout << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    // */
    /*
    cout << "  @Teste utilizando charClass`s" << endl << endl;
    
    try{
    cout << 6 << endl;
    cout << "Teste leu!" << endl;
    banco::addInst("[0-9] => #!stdout: leu!;");
    cout << "      "; banco::execInst("0"); cout << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl << endl; }
    
    banco::remLast();
    
    try{
    cout << 7 << endl;
    cout << "Teste leu!" << endl;
    banco::addInst("[0-9][0-9] => #!stdout: leu!;");
    cout << "      "; banco::execInst("00"); cout << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl << endl; }
    
    banco::remLast();
    
    cout << "  @Teste utilizando um objectClasse`s" << endl << endl;
    
    try{
    cout << 8 << endl;
    cout << "Teste 100" << endl;
    banco::addInst("(\"[0-9][0-9]*\")v; => #!stdout: @v;");
    cout << "      "; banco::execInst("100"); cout << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl << endl; }
    
    banco::remLast();
    
    cout << "  @Teste utilizando rotulos" << endl << endl;
    
    try{
    // Declaro o tipo inteiro:
    banco::addInst("inteiro: [0-9][0-9]*");
    }catch(const char* c){ cout << string("error: ") + c << endl << endl; }
    
    PRINT_LIST(); cout << endl;
    cout << "Lista de rotulos: " << endl;
    for(auto& a : banco::rotList()) cout << "  " << a << endl;
    cout << endl;
    
    try{
    cout << 9 << endl;
    cout << "Teste Inteiro Encontrado!: 100" << endl;
    banco::addInst("(inteiro)int; => #!stdout: Inteiro Encontrado!: @int;;");
    cout << "      "; banco::execInst("100"); cout << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl << endl; }
    
    try{
    cout << 10 << endl;
    cout << "Teste Inteiro Encontrado!: 1000!" << endl;
    banco::addInst("(inteiro)int;! => #!stdout: Inteiro Encontrado!: @int;!;");
    cout << "      "; banco::execInst("1000!"); cout << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl << endl; }
    
    try{
    cout << 11 << endl;
    cout << "Teste Inteiro Encontrado!: 1000!" << endl;
    banco::addInst("my (inteiro)int;! => #!stdout: Inteiro Encontrado!: @int;!;");
    cout << "      "; banco::execInst("my 1000!"); cout << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl << endl; }
    
    cout << "  @Testes com o driver talkin" << endl << endl;
    
    // try{
    // Instrução de adição genérica de instruções:
    // banco::addInst("(\"[!-~ \t\n]*\")v => #!stdout: texto: @v;");
    // }catch(const char* c){ cout << string("error: ") + c << endl << endl; }
    
    // try{
    // banco::execInst("ola");
    // }catch(const char* c){ cout << string("error: ") + c << endl << endl; }
    
    // Crio a instrução responsável por acessar o driver do talkin:
    try{
    banco
    ::addInst("\"#!talkin\\: (\"[!-~ \n\t]*\")texto;\" => #!talkin: @texto;");
    }catch(const char* c){ cout << string("error: ") + c << endl << endl; }
    
    PRINT_LIST(); cout << endl;
    cout << "Lista de rotulos: " << endl;
    for(auto& a : banco::rotList()) cout << "  " << a << endl;
    cout << endl;

    try{
    banco::execInst("#!talkin: testando");
    }catch(const char* c){ cout << string("error: ") + c << endl << endl; }
    
    try{
    cout << 1 << endl;
    cout << "Teste adição de inst" << endl;
    banco::execInst
    ("#!talkin: add some[ ](inteiro)n1;[ ]+[ ](inteiro)n2; => #!stdout: @n1; + @n2;");
    }catch(const char* c){ cout << string("error: ") + c << endl << endl; }

    PRINT_LIST(); cout << endl;
    cout << "Lista de rotulos: " << endl;
    for(auto& a : banco::rotList()) cout << "  " << a << endl;
    cout << endl;

    try{
    cout << 2 << endl;
    cout << "Teste 10" << endl;
    cout << "      "; banco::execInst("some 5 + 5");
    
    }catch(const char* c){ cout << string("error: ") + c << endl << endl; }
// */
  }
}

/* * * * * END TEST banco * * * * */
