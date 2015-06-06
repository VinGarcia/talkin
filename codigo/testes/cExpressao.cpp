#include "../banco.hpp"
#include <iostream>
#include "number.h"

using namespace std;

/* * * * * START TEST cExpressao * * * * */

int main(int argc, char* argv[])
{
  cExpressao exp;
  int pos;
  

#if NUMBER==cExpressao || NUMBER==1 || NUMBER==ALL
{
  cout << " * * * * * TESTE cExpressao() * * * * *\n\n";
  
  cout << "  @Testes com operandos numericos:" << endl << endl;
  
  //banco::addInst("(\"[0-9][0-9]*\")p; - p = 3 => #!stdout: works!");
  //banco::execInst("3");
  
  try{
  cout << 1 << endl;
  cout << "Teste \"0 0 <\"" << endl;
  exp = cExpressao("0 < 0", pos=0);
  cout << "      \"" << exp.str_raw() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 2 << endl;
  cout << "Teste \"0 0 ==\"" << endl;
  exp = cExpressao("0 == 0", pos=0);
  cout << "      \"" << exp.str_raw() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 3 << endl;
  cout << "Teste \"0 0 -\"" << endl;
  exp = cExpressao("0 - 0", pos=0);
  cout << "      \"" << exp.str_raw() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 4 << endl;
  cout << "Teste \"0 1 2 * +\"" << endl;
  exp = cExpressao("0 + 1 * 2", pos=0);
  cout << "      \"" << exp.str_raw() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 5 << endl;
  cout << "Teste \"0 1 * 2 +\"" << endl;
  exp = cExpressao("0 * 1 + 2", pos=0);
  cout << "      \"" << exp.str_raw() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 6 << endl;
  cout << "Teste \"0 1 2 * + 3 + 4 ==\"" << endl;
  exp = cExpressao("0 + 1 * 2 + 3 == 4", pos=0);
  cout << "      \"" << exp.str_raw() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 7 << endl;
  cout << "Teste \"0 1 2 3 * + == 4 +\"" << endl;
  exp = cExpressao("0 == 1 + 2 * 3 + 4", pos=0);
  cout << "      \"" << exp.str_raw() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 8 << endl;
  cout << "Teste \"0 ! 1 ||\"" << endl;
  exp = cExpressao("! 0 || 1", pos=0);
  cout << "      \"" << exp.str_raw() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 9 << endl;
  cout << "Teste \"1 2 3 4 * + ==\"" << endl;
  exp = cExpressao("1 == 2 + 3 * 4", pos=0);
  cout << "      \"" << exp.str_raw() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 10 << endl;
  cout << "Teste \"0 1 * 2 + 3 ==\"" << endl;
  exp = cExpressao("0 * 1 + 2 == 3", pos=0);
  cout << "      \"" << exp.str_raw() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 11 << endl;
  cout << "Teste \"0 1 ! 2 && ||\"" << endl;
  exp = cExpressao("0 || ! 1 && 2", pos=0);
  cout << "      \"" << exp.str_raw() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  cout << "  @Testes com operandos diversas:" << endl << endl;

  try{
  cout << 1 << endl;
  cout << "Teste '\"0\" \"1\" +'" << endl;
  exp = cExpressao("\"0\" + \"1\"", pos=0);
  cout << "      '" << exp.str_raw() << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 2 << endl;
  cout << "Teste '\"um\" \"dois\" +'" << endl;
  exp = cExpressao("\"um\" + \"dois\"", pos=0);
  cout << "      '" << exp.str_raw() << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 3 << endl;
  cout << "Teste 'a b +'" << endl;
  exp = cExpressao("a + b", pos=0);
  cout << "      '" << exp.str_raw() << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 4 << endl;
  cout << "Teste 'a.b c +'" << endl;
  exp = cExpressao("a.b + c", pos=0);
  cout << "      '" << exp.str_raw() << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 5 << endl;
  cout << "Teste 'a.b c.[0] +'" << endl;
  exp = cExpressao("a.b + c.[0]", pos=0);
  cout << "      '" << exp.str_raw() << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 6 << endl;
  cout << "Teste 'a.b.c.d.e[10].batata c.[0] +'" << endl;
  exp = cExpressao("a.b.c.d.e[10].batata + c.[0]", pos=0);
  cout << "      '" << exp.str_raw() << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
}
#endif

#if NUMBER==eval || NUMBER==2 || NUMBER==ALL
{
  cout << "  @Testes avaliando operações com strings:" << endl << endl;

  vars::cVar local, global;

  try{
  cout << 1.1 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("\"batata\" == \"batata\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 1.2 << endl;
  cout << "Teste ''" << endl;
  exp = cExpressao("\"batata\" == \"batatA\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 2.1 << endl;
  cout << "Teste ''" << endl;
  exp = cExpressao("\"batata\" != \"batata\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 2.2 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("\"batata\" != \"Batata\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 3.1 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("\"0\" < \"1\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 3.2 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("\"10\" < \"11\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 3.3 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("\"10.1\" < \"11\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 3.4 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("\"10.1\" < \"11.123\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 3.5 << endl;
  cout << "Teste ''" << endl;
  exp = cExpressao("\"10.1\" < \"10.1\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 3.6 << endl;
  cout << "Teste ''" << endl;
  exp = cExpressao("\"10.1\" < \"-10.1\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 3.7 << endl;
  cout << "Teste ''" << endl;
  exp = cExpressao("\"10.1q\" < \"10.1\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 3.8 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("\"10.1\" < \"10.1q\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 4.1 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("\"1\" > \"0\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 4.2 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("\"11\" > \"10\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 4.3 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("\"11\" > \"10.1\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 4.4 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("\"11.123\" > \"10.1\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 4.5 << endl;
  cout << "Teste ''" << endl;
  exp = cExpressao("\"10.1\" > \"10.1\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 4.6 << endl;
  cout << "Teste ''" << endl;
  exp = cExpressao("\"-10.1\" > \"10.1\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 4.7 << endl;
  cout << "Teste ''" << endl;
  exp = cExpressao("\"10.1\" > \"10.1q\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 4.8 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("\"10.1q\" > \"10.1\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 5.1 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("\"a\" && \"b\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 5.2 << endl;
  cout << "Teste ''" << endl;
  exp = cExpressao("\"a\" && \"\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 5.3 << endl;
  cout << "Teste ''" << endl;
  exp = cExpressao("\"\" && \"\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 6.1 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("\"a\" || \"b\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 6.2 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("\"a\" || \"\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 6.3 << endl;
  cout << "Teste ''" << endl;
  exp = cExpressao("\"\" || \"\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 7.1 << endl;
  cout << "Teste '7'" << endl;
  exp = cExpressao("\"4\" + \"3\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 7.2 << endl;
  cout << "Teste '7.1'" << endl;
  exp = cExpressao("\"7.0\" + \"0.1\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 7.3 << endl;
  cout << "Teste '7.1'" << endl;
  exp = cExpressao("\"7.0\" + \"1e-1\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 7.4 << endl;
  cout << "Teste ''" << endl;
  exp = cExpressao("\"7.0\" + \"1e-1q\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 8.1 << endl;
  cout << "Teste '7'" << endl;
  exp = cExpressao("\"10\" - \"3\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 8.2 << endl;
  cout << "Teste '7.1'" << endl;
  exp = cExpressao("\"7.2\" - \"0.1\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 8.3 << endl;
  cout << "Teste '7.1'" << endl;
  exp = cExpressao("\"7.2\" - \"1e-1\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 8.4 << endl;
  cout << "Teste ''" << endl;
  exp = cExpressao("\"7.2\" - \"1e-1q\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 9.1 << endl;
  cout << "Teste '7.1'" << endl;
  exp = cExpressao("\"3.55\" * \"2\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 9.2 << endl;
  cout << "Teste '7.1'" << endl;
  exp = cExpressao("\"355e-2\" * \"2\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 9.3 << endl;
  cout << "Teste '7.1'" << endl;
  exp = cExpressao("\"14.2\" * \"0.5\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 9.4 << endl;
  cout << "Teste '-7.1'" << endl;
  exp = cExpressao("\"14.2\" * \"-0.5\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 10.1 << endl;
  cout << "Teste '7.1'" << endl;
  exp = cExpressao("\"3.55\" / \"0.5\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 10.2 << endl;
  cout << "Teste '7.1'" << endl;
  exp = cExpressao("\"355e-2\" / \"0.5\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 10.3 << endl;
  cout << "Teste '7.1'" << endl;
  exp = cExpressao("\"14.2\" / \"2\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 10.4 << endl;
  cout << "Teste '-7.1'" << endl;
  exp = cExpressao("\"14.2\" / \"-2\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 11.1 << endl;
  cout << "Teste ''" << endl;
  exp = cExpressao("! \"teste\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 11.2 << endl;
  cout << "Teste ''" << endl;
  exp = cExpressao("! \"true\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 11.2 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("! \"\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 12 << endl;
  cout << "Teste '12'" << endl;
  exp = cExpressao("\"1\" + \"20e-1\" * \"3\" + \"5.0\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 13 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("\"true\" || \"\" == \"true\" && \"true\"", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  cout << "  @Testes avaliando operações com numeros:" << endl << endl;

  try{
  cout << 1 << endl;
  cout << "Teste '12'" << endl;
  exp = cExpressao("1 + 20e-1 * 3 + 5.0", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 2 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("true || false == true && true", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 3 << endl;
  cout << "Teste ''" << endl;
  exp = cExpressao("true || false == true && false", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 4 << endl;
  cout << "Teste 'true'" << endl;
  exp = cExpressao("false || false == true && false", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  cout << "  @Testes avaliando operações com variáveis:" << endl << endl;

  try{
  cout << 1 << endl;
  cout << "Teste '12'" << endl;
  exp = cExpressao("1 + 20e-1 * 3 + 5.0", pos=0);
  cout << "      '" << exp.eval(local,global) << "'" << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
}
#endif

  return 0;
}

/* * * * * END TEST cExpressao * * * * */
