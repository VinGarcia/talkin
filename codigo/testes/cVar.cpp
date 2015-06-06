#include "../padrao.hpp"
#include "../vars.hpp"
#include <iostream>
#include "number.h"

using namespace std;

/* * * * * START TEST cVar * * * * */

int main(int argc, char* argv[])
{
  vars::cVar* dad;
  vars::cVar* var;

  using vars::cVar;

#if NUMBER==cVar || NUMBER==1 || NUMBER==ALL
{
  cout << " * * * * * TESTE cVar() * * * * *\n\n";
  
  cout << "  @Testes com operandos numericos:" << endl << endl;
  
  // banco::addInst("(\"[0-9][0-9]*\")p; - p = 3 => #!stdout: works!");
  // banco::execInst("3");
  
  try{
  cout << 1 << endl;
  cout << "Teste '(nome:valor)0'" << endl;
  dad = new cVar("nome","valor"); var = dad;
  cout << "      '" << var->str() << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 2 << endl;
  cout << "Teste '(nome:valor)0 {(filho:primogenito)1}'" << endl;
  var->addChild("filho","primogenito");
  cout << "      '" << var->str() << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 3 << endl;
  cout << "Teste '(filho:primogenito)1'" << endl;
  var = var->getVar("filho");
  cout << "      '" << var->str() << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 4 << endl;
  cout << "Teste '(filho:primogenito)1 {(neto1:primogenito)2}'" << endl;
  var->addChild("neto1","primogenito");
  cout << "      '" << var->str() << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 5 << endl;
  cout << "Teste '(filho:primogenito)1 {(neto1:primogenito)2,(neto2:caçula)2}'" << endl;
  var->addChild("neto2","caçula");
  cout << "      '" << var->str() << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 6 << endl;
  cout << "Teste '(nome:valor)0 {(filho:primogenito)1 {(neto1:primogenito)2,(neto2:caçula)2}}'" << endl;
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
  cout << "Teste '(neto2:caçula)2'" << endl;
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
  cout << "Teste '(nome:valor)0 {(filho:primogenito)1 {(neto1:primogenito)2,(neto2:caçula)2,(neto3:nenem)2}}'" << endl;
  dad->addVar("filho.neto3","nenem");
  cout << "      '" << dad->str() << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 2 << endl;
  cout << "Teste '(nome:valor)0 {(filho:primogenito)1 {(neto1:primogenito)2,(neto2:caçula)2,(neto3:nenem)2,(neto4:)2 {(bisneto1:embrião)3}}}'" << endl;
  dad->addVar("filho.neto4.bisneto1","embrião");
  cout << "      '" << dad->str() << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 3 << endl;
  cout << "Teste '(doce:de_leite)0 {(filho:)1 {(alpha:)2 {(biscoito:recheio)3}}}'" << endl;
  dad = new cVar("doce","de_leite"); dad->addVar("filho.alpha.biscoito","recheio");
  cout << "      '" << dad->str() << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
}
#endif

#if NUMBER==arrayClass || NUMBER==4 || NUMBER==ALL
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
  cout << "Teste '(:tete)0 {(a:te)1,(b:)1,(c:te)1}'" << endl;
  var = new cVar(*pmv);
  cout << "      '" << var->str() << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 1 << endl;
  cout << "Teste '(:tesste)0 {(a:tes)1,(b:)1,(c:ste)1}'" << endl;
  array.match("tesste",0); var = new cVar(*pmv);
  cout << "      '" << var->str() << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
}
#endif
  
  return 0;
}

/* * * * * END TEST cVar * * * * */