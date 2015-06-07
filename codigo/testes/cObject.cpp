#include "../padrao.hpp"
#include "../vars.hpp"
#include <iostream>
#include "number.h"

using namespace std;

/* * * * * START TEST cObject * * * * */

int main(int argc, char* argv[])
{
  vars::cObject* dad;
  vars::cObject* var;

  using vars::cObject;

#if NUMBER==cObject || NUMBER==1 || NUMBER==ALL
{
  cout << " * * * * * TESTE cObject() * * * * *\n\n";
  
  cout << "  @Testes com operandos numericos:" << endl << endl;
  
  // banco::addInst("(\"[0-9][0-9]*\")p; - p = 3 => #!stdout: works!");
  // banco::execInst("3");
  
  try{
  cout << 1 << endl;
  cout << "Teste '{$:'valor'}'" << endl;
  dad = new cObject("valor"); var = dad;
  cout << "      '" << var->str() << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 2 << endl;
  cout << "Teste '{$:'valor',filho:{$:'primogenito'}}'" << endl;
  var->addChild("filho","primogenito");
  cout << "      '" << var->str() << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 3 << endl;
  cout << "Teste '{$:'primogenito'}'" << endl;
  var = var->getVar("filho");
  cout << "      '" << var->str() << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 4 << endl;
  cout << "Teste '{$:'primogenito',neto1:{$:'primogenito'}}'" << endl;
  var->addChild("neto1","primogenito");
  cout << "      '" << var->str() << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 5 << endl;
  cout << "Teste '{$:'primogenito',neto1:{$:'primogenito'},neto2:{$:'caçula'}}'" << endl;
  var->addChild("neto2","caçula");
  cout << "      '" << var->str() << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 6 << endl;
  cout << "Teste '{$:'valor',filho:{$:'primogenito',neto1:{$:'primogenito'},neto2:{$:'caçula'}}}'" << endl;
  var=dad;
  cout << "      '" << var->str() << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
}
#endif

#if NUMBER==getVar || NUMBER==2 || NUMBER==ALL
{
  cout << "  @Testes com a função getVar()" << endl << endl;

  try{
  cout << 1 << endl;
  cout << "Teste '{$:'caçula'}'" << endl;
  var = dad->getVar("filho.neto2");
  cout << "      '" << var->str() << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 2 << endl;
  cout << "Teste null" << endl;
  var = dad->getVar("filho.neto3");
  cout << "      " << (!var?"null":"not_null") << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
}
#endif

#if NUMBER==addVar || NUMBER==3 || NUMBER==ALL
{
  cout << "  @Testes com a função addVar()" << endl << endl;

  try{
  cout << 1 << endl;
  cout << "Teste '{$:'valor',filho:{$:'primogenito',neto1:{$:'primogenito'},neto2:{$:'caçula'},neto3:{$:'nenem'}}}'" << endl;
  dad->addVar("filho.neto3","nenem");
  cout << "      '" << dad->str() << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 2 << endl;
  cout << "Teste '{$:'valor',filho:{$:'primogenito',neto1:{$:'primogenito'},neto2:{$:'caçula'},neto3:{$:'nenem'},neto4:{bisneto1:{$:'embrião'}}}}'" << endl;
  dad->addVar("filho.neto4.bisneto1","embrião");
  cout << "      '" << dad->str() << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 3 << endl;
  cout << "Teste '{$:'doce_de_leite',filho:{alpha:{biscoito:{$:'recheio'}}}}'" << endl;
  dad = new cObject("doce_de_leite"); dad->addVar("filho.alpha.biscoito","recheio");
  cout << "      '" << dad->str() << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
}
#endif

#if NUMBER==cVar || NUMBER==4 || NUMBER==ALL
{
  cout << "  @Testes com as construtoras que copiam pMatch::cVars" << endl << endl;

  auto array = pMatch::arrayClass("(\"tes\",\"te\")a(\"s\",\"\")b(\"te\",\"ste\")c");
  pMatch::cVar* pmv;

  array.match("tete",0);
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
}
#endif
  
  return 0;
}

/* * * * * END TEST cObject * * * * */