#include "../banco.hpp"
#include <iostream>
#include "number.h"

#include "catch.hpp"
#include "catch_lower.hpp"

using namespace std;

/* * * * * START TEST cSignificado * * * * */

TEST_CASE("cSignificado") {
  using namespace pMatch;
  cSignificado sig;
  int pos;
  
  #if NUMBER==cSignificado || NUMBER==1 || NUMBER==ALL
  {

    cout << " * * * * * TESTE cSignificado() * * * * *\n\n";
    
    cout << "  @Testes com cSignificado`s:" << endl << endl;
    
    try{
    cout << 1 << endl;
    cout << "Teste \"testando\";" << endl;
    sig = cSignificado("testando", pos=0);
    cout << "      " << sig.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    try{
    cout << 2 << endl;
    cout << "Teste \"testando\";" << endl;
    sig = cSignificado("testando;", pos=0);
    cout << "      " << sig.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    try{
    cout << 3 << endl;
    cout << "Teste \"testando\";" << endl;
    sig = cSignificado("\"testando\";", pos=0);
    cout << "      " << sig.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    try{
    cout << 4 << endl;
    cout << "Teste \"testando\";" << endl;
    sig = cSignificado("    \"testando\";", pos=0);
    cout << "      " << sig.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    try{
    cout << 5 << endl;
    cout << "Teste \"testando\";" << endl;
    sig = cSignificado("\"testando\"    ;", pos=0);
    cout << "      " << sig.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    try{
    cout << 6 << endl;
    cout << "Teste \"testando\";" << endl;
    sig = cSignificado("\"testando\";    ", pos=0);
    cout << "      " << sig.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    try{
    cout << 7 << endl;
    cout << "Teste \"testando\";" << endl;
    sig = cSignificado("    testando    ;    ", pos=0);
    cout << "      " << sig.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    cout << "  @Teste com drivers:" << endl << endl;
    
    try{
    cout << 8 << endl;
    cout << "Teste #!talkin: \"testando\";" << endl;
    sig = cSignificado("#!talkin: testando;", pos=0);
    cout << "      " << sig.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    try{
    cout << 9 << endl;
    cout << "Teste #!talkin: \"testando\";" << endl;
    sig = cSignificado("#!talkin:testando    ;    ", pos=0);
    cout << "      " << sig.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    try{
    cout << 10 << endl;
    cout << "Teste #!talkin: \"testando\";" << endl;
    sig = cSignificado("#!  talkin  : testando;", pos=0);
    cout << "      " << sig.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    try{
    cout << 11 << endl;
    cout
    << "Este teste deveria lançar um erro." << endl
    << "por enquanto o comportamento é aceitável porque a validade()" << endl
    << "Ainda não está implementada, porém irei deixa-lo aqui como um lembrete"
    << endl;
    cout << "Teste #!talkin: \"testando\";" << endl;
    sig = cSignificado("#!  tal  kin  : testando;", pos=0);
    cout << "      " << sig.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    try{
    cout << 12 << endl;
    cout << "Teste #!talkin: \"testando\";" << endl;
    sig = cSignificado("      #!talkin: testando;", pos=0);
    cout << "      " << sig.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    cout << "  @Teste variando o pos:" << endl << endl;
    
    try{
    cout << 13 << endl;
    cout << "Teste #!talkin: \"testando\";" << endl;
    sig = cSignificado("#!talkin: teste;", pos=0);
    sig = cSignificado("#!talkin: teste; #!talkin: testando;", pos);
    cout << "      " << sig.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    try{
    cout << 14 << endl;
    cout << "Teste \"teste\";" << endl;
    sig = cSignificado("#!talkin: teste; #!talkin: testando; teste", pos);
    cout << "      " << sig.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
  }
  #endif
}

/* * * * * END TEST cSignificado * * * * */
