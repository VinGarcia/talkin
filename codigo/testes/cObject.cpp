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

  using vars::cObject;

  #if NUMBER==cObject || NUMBER==1 || NUMBER==ALL
  {
    cout << " * * * * * TESTE cObject() * * * * *\n\n";
    
    cout << "  @Testes basicos com cObject:" << endl << endl;
    
    try{
    cout << 1 << endl;
    cout << "Teste '{$:'valor'}'" << endl;
    dad = new cObject("valor"); var = dad;
    cout << "      '" << var->str() << "'" << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 2 << endl;
    cout << "Teste '{$:'valor'}'" << endl;
    dad = new cObject(string("valor")); var = dad;
    cout << "      '" << var->str() << "'" << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 3 << endl;
    cout << "Teste '{}'" << endl;
    dad = new cObject(""); var = dad;
    cout << "      '" << var->str() << "'" << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 4 << endl;
    cout << "Teste '{}'" << endl;
    dad = new cObject(string("")); var = dad;
    cout << "      '" << var->str() << "'" << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
  }
  #endif

  #if NUMBER==cVar || NUMBER==2 || NUMBER==ALL
  {
    cout << "  @Testes com as construtoras que copiam pMatch::cVars" << endl << endl;

    auto array = pMatch::arrayClass("(\"tes\",\"te\")a(\"s\",\"\")b(\"te\",\"ste\")c");
    pMatch::cVar* pmv;

    pMatch::arrayClass array2;
    try{
    array2 = pMatch::arrayClass("(\"(\"teste\")b\", \"tes(\"te\")c\")a");
    }
    catch(char const* e) { cout << e << endl; }

    // Create a matched variable:
    array.match("tete",0);
    // Save it on pmv:
    pmv = &(array.var);

    // cout << "pMatch::var: " << endl;
    // cout << pmv.str() << endl;

    try{
    cout << 1 << endl;
    cout << "Teste '{$:'tete',a:{$:'te'},b:{},c:{$:'te'}}'" << endl;
    var = new cObject(*pmv);
    cout << "      '" << var->str() << "'" << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    try{
    cout << 2 << endl;
    cout << "Teste '{$:'tesste',a:{$:'tes'},b:{},c:{$:'ste'}}'" << endl;
    array.match("tesste",0); var = new cObject(*pmv);
    cout << "      '" << var->str() << "'" << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 3 << endl;
    cout << "Teste '{$:'teste',a:{$:'teste',c:{$:'te'}}}'" << endl;
    array2.match("teste",0); var = new cObject(array2.var);
    cout << "      '" << var->str() << "'" << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
  }
  #endif

  #if NUMBER==child || NUMBER==1 || NUMBER==ALL
  {
    cout << "  @Testes com a função `child()` sem recursão" << endl << endl;

    var = new cObject("valor");
    dad = var;

    try{
    cout << 1 << endl;
    cout << "Teste '{$:'valor',filho:{$:'primogenito'}}'" << endl;
    var->child("filho",true) = "primogenito";
    cout << "      '" << var->str() << "'" << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 2 << endl;
    cout << "Teste '{$:'valor',filho:{$:'caçula'}}'" << endl;
    var->child("filho",true) = cObject("caçula");
    cout << "      '" << var->str() << "'" << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 3 << endl;
    cout << "Teste '{$:'caçula'}'" << endl;
    var = &var->child("filho");
    cout << "      '" << var->str() << "'" << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 4 << endl;
    cout << "Teste '{$:'caçula',neto1:{$:'primogenito'}}'" << endl;
    var->child("neto1",true) = "primogenito";
    cout << "      '" << var->str() << "'" << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 5 << endl;
    cout << "Teste '{$:'caçula',neto1:{$:'primogenito'},neto2:{$:'caçula'}}'" << endl;
    var->child("neto2",true) = "caçula";
    cout << "      '" << var->str() << "'" << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 6 << endl;
    cout << "Teste '{$:'valor',filho:{$:'caçula',neto1:{$:'primogenito'},neto2:{$:'caçula'}}}'" << endl;
    var=dad;
    cout << "      '" << var->str() << "'" << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
  }
  #endif

    

  #if NUMBER==child || NUMBER==3 || NUMBER==ALL
  {
    cout << "  @Testes com a função `child()` para leitura" << endl << endl;

    vars::cObject aux;
    dad = new cObject("valor");
    dad->child("filho", true) = "caçula";
    dad->child("filho").child("neto2", true) = "caçula";

    try{
    cout << 1 << endl;
    cout << "Teste '{$:'caçula'}'" << endl;
    aux = dad->child("filho.neto2");
    cout << "      '" << aux.str() << "'" << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 2 << endl;
    cout << "Teste null" << endl;
    aux = dad->child("filho.neto3");
    cout << "      " << (aux==cObject::undefined?"null":"not_null") << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
  }
  #endif

  #if NUMBER==child || NUMBER==4 || NUMBER==ALL
  {
    cout << "  @Testes com a função `child()` para escrita" << endl << endl;

    dad = new cObject("valor");
    dad->child("filho", true) = "primogenito";
    dad->child("filho").child("neto2", true) = "caçula";

    try{
    cout << 1 << endl;
    cout << "Teste '{$:'valor',filho:{$:'primogenito',neto2:{$:'caçula'},neto3:{$:'nenem'}}}'" << endl;
    dad->child("filho.neto3",true) = "nenem";
    cout << "      '" << dad->str() << "'" << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 2 << endl;
    cout << "Teste '{$:'valor',filho:{$:'primogenito',neto2:{$:'caçula'},neto3:{$:'nenem'},neto4:{bisneto1:{$:'embrião'}}}}'" << endl;
    dad->child("filho.neto4.bisneto1",true) = "embrião";
    cout << "      '" << dad->str() << "'" << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 3 << endl;
    cout << "Teste '{$:'doce_de_leite',filho:{alpha:{biscoito:{$:'recheio'}}}}'" << endl;
    dad = new cObject("doce_de_leite"); dad->child("filho.alpha.biscoito",true) = "recheio";
    cout << "      '" << dad->str() << "'" << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
  }
  #endif

  #if NUMBER==child || NUMBER==5 || NUMBER==ALL
  {
    cout << "  @Testes de ERRO com a função `child()`" << endl << endl;

    dad = new cObject("valor");
    dad->child("filho", true) = "primogenito";
    dad->child("filho").child("neto2", true) = "caçula";

    try{
    cout << 1 << endl;
    cout << "Teste OK" << endl;
    cObject::null = "batatinha";
    cout << "      OK" << endl << endl;
    }catch(const char* c){ cout << "      ERROR" << endl << endl; }

    try{
    cout << 2.1 << endl;
    cout << "Teste OK" << endl;
    dad->child("");
    cout << "      OK" << endl << endl;
    }catch(const char* c){ cout << "      ERROR" << endl << endl; }

    try{
    cout << 2.2 << endl;
    cout << "Teste OK" << endl;
    dad->child("",true);
    cout << "      OK" << endl << endl;
    }catch(const char* c){ cout << "      ERROR" << endl << endl; }
    
    try{
    cout << 3.1 << endl;
    cout << "Teste ERROR" << endl;
    dad->child("[21");
    cout << "      OK" << endl << endl;
    }catch(const char* c){ cout << "      ERROR" << endl << endl; }
    
    try{
    cout << 3.2 << endl;
    cout << "Teste ERROR" << endl;
    dad->child("filho1.[10]", true);
    cout << "      OK" << endl << endl;
    }catch(const char* c){ cout << "      ERROR" << endl << endl; }

    try{
    cout << 3.3 << endl;
    cout << "Teste OK" << endl;
    dad->child(".filho1[10]");
    cout << "      OK" << endl << endl;
    }catch(const char* c){ cout << "      ERROR" << endl << endl; }
    
    try{
    cout << 3.4 << endl;
    cout << "Teste OK" << endl;
    dad->child("    .    filho1    [    10    ]    ");
    cout << "      OK" << endl << endl;
    }catch(const char* c){ cout << "      ERROR" << endl << endl; }
    
    try{
    cout << 3.5 << endl;
    cout << "Teste ERROR" << endl;
    dad->child("    .    fil ho1    [    10    ]    ");
    cout << "      OK" << endl << endl;
    }catch(const char* c){ cout << "      ERROR" << endl << endl; }
    
    try{
    cout << 3.6 << endl;
    cout << "Teste ERROR" << endl;
    dad->child("    .    filho1    [    1 0    ]    ");
    cout << "      OK" << endl << endl;
    }catch(const char* c){ cout << "      ERROR" << endl << endl; }
    
    try{
    cout << 4.1 << endl;
    cout << "Teste ERROR" << endl;
    dad->child("filho!", true);
    cout << "      OK" << endl << endl;
    }catch(const char* c){ cout << "      ERROR" << endl << endl; }
    
    try{
    cout << 4.2 << endl;
    cout << "Teste ERROR" << endl;
    dad->child("#hashtag");
    cout << "      OK" << endl << endl;
    }catch(const char* c){ cout << "      ERROR" << endl << endl; }
    
    try{
    cout << 5.1 << endl;
    cout << "Teste ERROR" << endl;
    dad->child("filho1[10[]");
    cout << "      OK" << endl << endl;
    }catch(const char* c){ cout << "      ERROR" << endl << endl; }
    
    try{
    cout << 5.2 << endl;
    cout << "Teste ERROR" << endl;
    dad->child("filho1[0.9]");
    cout << "      OK" << endl << endl;
    }catch(const char* c){ cout << "      ERROR" << endl << endl; }
    
    try{
    cout << 5.3 << endl;
    cout << "Teste ERROR" << endl;
    dad->child("filho1.0.9].origami");
    cout << "      OK" << endl << endl;
    }catch(const char* c){ cout << "      ERROR" << endl << endl; }

    try{
    cout << 5.4 << endl;
    cout << "Teste OK" << endl;
    dad->child("filho1[9].origami");
    cout << "      OK" << endl << endl;
    }catch(const char* c){ cout << "      ERROR" << endl << endl; }
    
    try{
    cout << 5.5 << endl;
    cout << "Teste ERROR" << endl;
    dad->child("filho1[09]origami");
    cout << "      OK" << endl << endl;
    }catch(const char* c){ cout << "      ERROR" << endl << endl; }

    try{
    cout << 5.6 << endl;
    cout << "Teste ERROR" << endl;
    dad->child("filho1[09]   origami");
    cout << "      OK" << endl << endl;
    }catch(const char* c){ cout << "      ERROR" << endl << endl; }
    
    try{
    cout << 5.7 << endl;
    cout << "Teste OK" << endl;
    dad->child("filho1[09][origami]");
    cout << "      OK" << endl << endl;
    }catch(const char* c){ cout << "      ERROR" << endl << endl; }
    
    try{
    cout << 5.8 << endl;
    cout << "Teste OK" << endl;
    dad->child("filho1[09]");
    cout << "      OK" << endl << endl;
    }catch(const char* c){ cout << "      ERROR" << endl << endl; }
    
    try{
    cout << 5.9 << endl;
    cout << "Teste ERROR" << endl;
    dad->child("filho1[09]   ].btt");
    cout << "      OK" << endl << endl;
    }catch(const char* c){ cout << "      ERROR" << endl << endl; }

    try{
    cout << 6 << endl;
    cout << "Teste OK" << endl;
    dad->child("p");
    cout << "      OK" << endl << endl;
    }catch(const char* c){ cout << "      ERROR" << endl << endl; }
  }
  #endif
}

/* * * * * END TEST cObject * * * * */










