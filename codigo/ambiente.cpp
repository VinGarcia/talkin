#include <fstream>
#include <iostream>
#include <stdlib.h>

#include "ambiente.hpp"

using namespace std;
using namespace ambiente;

// Variáveis globais
vars::cVar ambiente::global;
// Drivers do talkin:
std::map<std::string, driver*> ambiente::drivers;

void inicializa_banco()
{
  using namespace pMatch;
  
  // Instruções básicas do sistema:
  //banco::instrucoes[0][3]= cInst("anything: [!-~ \n\t]*",3);
// TODO: fazer ele reconhecer a instrução abaixo usando o objeto "anything"
  banco::instrucoes[0][2]=
  cInst("[!-~ \n\t]* => #!stdout: Comando nao reconhecido.\n  Atualmente so existem os comandos: 'ola', 'help' e 'exit'",2);
  banco::instrucoes[0][1]= cInst("blank: [ \t\n]*",1);
  //banco::instrucoes[0][0]=
  //cInst("#!talkin:[~-! \n\t]* => #!stdout: teste", 0);
  //cInst("#!talkin:(blank)(\"add\",\"rem\",\"show\")(blank)(anything) => #!stdout: teste", 0);
}

void readVocab(ifstream& file)
{
  bool reading=false;
  string line;
  string inst;
  cInst* aux;
  
  try {
    while(getline(file,line))
    {
      if(!reading && line != string("")) reading = true;
      if(reading) inst += " " + line;
      if(line==string(""))
      {
        try{
          aux = banco::addInst(inst);
        }catch(const char* c)
        {throw std::make_pair(c,line);}
        cout << "inst: " << aux->str();
        cout << endl << endl;
        inst = string("");
      }
    }
    if(inst != string(""))
      try{
        aux = banco::addInst(inst);
      }catch(const char* c)
      {throw std::make_pair(c,line);}
    cout << "inst: " << aux->str();
    cout << endl << endl;
  }
  catch(std::pair<const char*,std::string> c) {
    cout << c.first << endl;
    cout << "line: " << c.second << endl;
    throw "Erro no código do arquivo vocabulario.talk";
  }
}

// Retorna true se o contexto é válido, e false caso contrário.
void interface()
{
  string line;
  
  cout << "Talkin 0.9 by Vinícius Garcia" << endl;
  
  while(1)
  {
    getline(cin, line);
    if(line==string("exit")) break;
    
    try{
    banco::execInst(line);
    }catch(const char* c){ cout << string("erro: ") + c << endl; }
  }
}

/* * * * * START TEST objectClass * * * * */

// Teste do objectClass:
// Descomente para testar:
 /*
int main(int argc, char* argv[])
{
  using namespace pMatch;
  
  objectClass oc;
  int pos;
  
  cout << " * * * * * TESTE objectClass() * * * * *\n\n";
  
  cout << "  @Testes com objectClass(string):" << endl << endl;
  
  try{
  cout << 1 << endl;
  cout << "Teste \"verbo\"" << endl;
  oc = objectClass("verbo");
  cout << "      \"" << oc.str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 2 << endl;
  cout << "Teste \"verbo dois\"" << endl;
  oc = objectClass("verbo      dois");
  cout << "      \"" << oc.str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 3 << endl;
  cout << "Teste \"verbo tres\"" << endl;
  oc = objectClass("    verbo      tres");
  cout << "      \"" << oc.str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 4 << endl;
  cout << "Teste \"verbo quatro\"" << endl;
  oc = objectClass("    verbo      quatro    ");
  cout << "      \"" << oc.str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  cout << "  @Testes com objectClass(string, pos):" << endl << endl;
  
  try{
  cout << 5 << endl;
  cout << "Teste \"verbo\"" << endl;
  oc = objectClass("verbo", pos=0);
  cout << "      \"" << oc.str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 6 << endl;
  cout << "Teste \"verbo\"" << endl;
  oc = objectClass("averbo", pos=1);
  cout << "      \"" << oc.str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 7 << endl;
  cout << "Teste \"verbo\"" << endl;
  oc = objectClass("  verbo  ", pos=1);
  cout << "      \"" << oc.str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 8 << endl;
  cout << "Teste \"verbo\"" << endl;
  oc = objectClass("verbo, verbula", pos=0);
  cout << "      \"" << oc.str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 9 << endl;
  cout << "Teste \"verbo\"" << endl;
  oc = objectClass("Verbo, verbo  ) amacada", pos);
  cout << "      \"" << oc.str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  cout << "  @Testes de validação do objectClass():" << endl << endl;
  
  try{
  cout << 10 << endl;
  cout << "Teste throwed" << endl;
  oc = objectClass("Verb,o");
  cout << "      \"" << oc.str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << "      " << "throwed" << endl << endl; }
  
  try{
  cout << 11 << endl;
  cout << "Teste throwed" << endl;
  oc = objectClass("Verb:o");
  cout << "      \"" << oc.str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << "      " << "throwed" << endl << endl; }
  
  try{
  cout << 12 << endl;
  cout << "Teste throwed" << endl;
  oc = objectClass("Verb(o");
  cout << "      \"" << oc.str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << "      " << "throwed" << endl << endl; }
  
  try{
  cout << 13 << endl;
  cout << "Teste throwed" << endl;
  oc = objectClass("Verb)o");
  cout << "      \"" << oc.str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << "      " << "throwed" << endl << endl; }
  
  try{
  cout << 14 << endl;
  cout << "Teste not throwed" << endl;
  oc = objectClass("Verb.!@#$%^&*-'\"\t\n o");
  cout << "      " << "not throwed" << endl << endl;
  }catch(const char* c){ cout << "      " << "throwed" << endl << endl; }
  
  return 0;
}
// */

// Teste do objectClass:
// Descomente para testar:
 /*
int main(int argc, char* argv[])
{
  using namespace pMatch;
  
  objectClass oc = objectClass("teste");
  
  cout << " * * * * * TESTE objectClass() * * * * *\n\n";
  
  cout << "  @Testes com objectClass(string):" << endl << endl;
  
  try{
  cout << 1 << endl;
  cout << "Teste \"\"" << endl;
  oc.match("testando",0);
  cout << "      \"" << oc.getMatch().str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl << endl; }
  
  banco::addInst("teste: testando => #!stdout: working");
  
  try{
  cout << 2 << endl;
  cout << "Teste \"testando\"" << endl;
  oc.match("testando",0);
  cout << "      \"" << oc.getMatch().str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl << endl; }
  
  try{
  cout << 2.1 << endl;
  cout << "Teste \"testando\"" << endl;
  oc.match("testestando",3);
  cout << "      \"" << oc.getMatch().str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl << endl; }
  
  try{
  cout << 2.2 << endl;
  cout << "Teste \"testando\"" << endl;
  oc.match("testestandoagora",3);
  cout << "      \"" << oc.getMatch().str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl << endl; }
  
  banco::addInst("teste: test => #!stdout: working");
  
  try{
  cout << 3 << endl;
  cout << "Teste \"testando\"" << endl;
  oc.match("testestandoagora",3);
  cout << "      \"" << oc.getMatch().str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl << endl; }
  
  return 0;
}
// */

/* * * * * END TEST objectClass * * * * */

/* * * * * START TEST cRotulo * * * * */

// Teste do cRotulo:
// Descomente para testar:
 /*
// TODO: Elaborar esse teste.
int main(int argc, char* argv[])
{
  using namespace pMatch;
  
  cRotulo* rot = &(banco::rotulos["teste"] = cRotulo("teste"));
  
  cout << " * * * * * TESTE cRotulo.match() * * * * *\n\n";
  
  cout << "  @Testes com cRotulo(string):" << endl << endl;
  
  try{
  cout << 1 << endl;
  cout << "Teste \"\"" << endl;
  rot->match("testando",0);
  cout << "      \"" << rot->getMatch().str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl << endl; }
  
  banco::addInst("teste: testando => #!stdout: working");
  
  try{
  cout << 2 << endl;
  cout << "Teste \"testando\"" << endl;
  rot->match("testando",0);
  cout << "      \"" << rot->getMatch().str() << '"' << endl; cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl << endl; }
  
  return 0;
}
// */

/* * * * * END TEST cRotulo * * * * */

/* * * * * START TEST banco * * * * */

// Teste do banco:
// Descomente para testar:
 /*
int main(int argc, char* argv[])
{
  using namespace pMatch;
  
  cout << " * * * * * TESTE banco::addInst() * * * * *\n\n";
  
  cout << "  @Testes com banco::addInst:" << endl << endl;
  
  try{
  cout << 1 << endl;
  cout << "Teste inst[1][0]: «rot: \"pad\" - contx => \"sig\";»" << endl;
  banco::addInst("rot: pad - contx => sig");
  for(auto& a : banco::strList()) cout << "      " << a << endl; cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 2 << endl;
  cout << "Teste inst[1][0]: «rot: \"pad\" - contx => \"sig\";»" << endl;
  cout << "Teste inst[1][1]: «rot2: \"pad2\" - contx2 => \"sig2\";»" << endl;
  banco::addInst("rot2: pad2 - contx2 => sig2");
  for(auto& a : banco::strList()) cout << "      " << a << endl; cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  cout << "  @Teste com o remInst()" << endl << endl;
  
  try{
  cout << 3 << endl;
  cout << "Teste inst[1][0]: «rot: \"pad\" - contx => \"sig\";»" << endl;
  banco::remInst(1);
  for(auto& a : banco::strList()) cout << "      " << a << endl; cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  cout << "  @Teste com o execInst()" << endl << endl;
  
  // Adicionando o driver stdout e talkin para a lista de drivers:
  drivers["stdout"] = new driver_stdout();
  drivers["talkin"] = new driver_talkin();
  
  try{
  cout << 4 << endl;
  cout << "Teste Hello World!" << endl;
  banco::addInst("[Oo]la! => #!stdout: Hello World!");
  cout << "      "; banco::execInst("Ola!"); cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  try{
  cout << 4.1 << endl;
  cout << "Teste Hello World!" << endl;
  banco::addInst("start! => ola!");
  cout << "      "; banco::execInst("start!"); cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  cout << "  @Teste utilizando o valor de uma variável" << endl << endl;
  
  try{
  cout << 5 << endl;
  cout << "Teste hello world!" << endl;
  banco::addInst("(\"[Hh]ello [wW]orld!*\")v; => #!stdout: @v;");
  cout << "      "; banco::execInst("hello world!"); cout << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
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
  
  for(auto& a : banco::strList()) cout << "      " << a << endl; cout << endl;
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
  
//  try{
  // Instrução de adição genérica de instruções:
//  banco::addInst("(\"[!-~ \t\n]*\")v => #!stdout: texto: @v;");
//  }catch(const char* c){ cout << string("error: ") + c << endl << endl; }
  
  //try{
  //banco::execInst("ola");
  //}catch(const char* c){ cout << string("error: ") + c << endl << endl; }
  
  // Crio a instrução responsável por acessar o driver do talkin:
  try{
  banco
  ::addInst("\"#!talkin\\: (\"[!-~ \n\t]*\")texto;\" => #!talkin: @texto;");
  }catch(const char* c){ cout << string("error: ") + c << endl << endl; }
  
  for(auto& a : banco::strList()) cout << "      " << a << endl; cout << endl;
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

  for(auto& a : banco::strList()) cout << "      " << a << endl; cout << endl;
  cout << "Lista de rotulos: " << endl;
  for(auto& a : banco::rotList()) cout << "  " << a << endl;
  cout << endl;

  try{
  cout << 2 << endl;
  cout << "Teste 10" << endl;
  cout << "      "; banco::execInst("some 5 + 5");
  
  }catch(const char* c){ cout << string("error: ") + c << endl << endl; }
  
  return 0;
}
// */

/* * * * * START TEST banco * * * * */

/* * * * * START TEST cSignificado * * * * */

// Teste do cSignificado:
// Descomente para testar:
 /*
int main(int argc, char* argv[])
{
  using namespace pMatch;
  cSignificado sig;
  int pos;
  
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
  
  return 0;
}
// */

/* * * * * END TEST cSignificado * * * * */

/* * * * * START TEST cExpressao * * * * */

// Teste do cExpressao:
// Descomente para testar:
 /*
int main(int argc, char* argv[])
{
  cExpressao exp;
  int pos;
  
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

  return 0;
}

// */

/* * * * * END TEST cExpressao * * * * */

/* * * * * START TEST cContexto * * * * */

// Teste do cContexto:
// Descomente para testar:
 /*
int main(int argc, char* argv[])
{
  using namespace pMatch;
  cContexto con;
  int pos;
  
  cout << " * * * * * TESTE cContexto() * * * * *\n\n";
  
  cout << "  @Testes com cContextos`s:" << endl << endl;
  
  //banco::addInst("(\"[0-9][0-9]*\")p; - p = 3 => #!stdout: works!");
  //banco::execInst("3");
  
  try{
  cout << 1 << endl;
  cout << "Teste \"0 = 0\"" << endl;
  con = cContexto("0 = 0", pos=0);
  cout << "      \"" << con.str() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 1.1 << endl;
  cout << "Teste \"10 = 10\"" << endl;
  con = cContexto("10 = 10", pos=0);
  cout << "      \"" << con.str() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 1.2 << endl;
  cout << "Teste \"10 = 10\"" << endl;
  con = cContexto("10= 10", pos=0);
  cout << "      \"" << con.str() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 1.3 << endl;
  cout << "Teste \"10 = 10\"" << endl;
  con = cContexto("10 =10", pos=0);
  cout << "      \"" << con.str() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 1.4 << endl;
  cout << "Teste \"10 = 10\"" << endl;
  con = cContexto(" 10 = 10", pos=0);
  cout << "      \"" << con.str() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 1.5 << endl;
  cout << "Teste \"10 = 10\"" << endl;
  con = cContexto("10 = 10 ", pos=0);
  cout << "      \"" << con.str() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 2 << endl;
  cout << "Teste \"0 = 0\"" << endl;
  con = cContexto("\"0\" = 0", pos=0);
  cout << "      \"" << con.str() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  try{
  cout << 3 << endl;
  cout << "Teste \"var = 0\"" << endl;
  con = cContexto("var = 0", pos=0);
  cout << "      \"" << con.str() << '"' << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }

  cout << "      @Teste da função eval():" << endl << endl;

  // Criando variáveis locais:
  vars::cVar v = vars::cVar();
  v.subvars["var"] = vars::cVar();
  v.subvars["var"].nome = "var";
  v.subvars["var"].valor = "0";
  
  try{
  cout << 4 << endl;
  cout << "Teste 1" << endl;
  con = cContexto("var = 0", pos=0);
  cout << "      " << con.eval(v,ambiente::global) << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  v.subvars["var"].valor="1";
  try{
  cout << 4.1 << endl;
  cout << "Teste 0" << endl;
  con = cContexto("var = 0", pos=0);
  cout << "      " << con.eval(v,ambiente::global) << endl << endl;
  }catch(const char* c){ cout << string("error: ") + c << endl; }
  
  return 0;
}
// */

/* * * * * END TEST cContexto * * * * */