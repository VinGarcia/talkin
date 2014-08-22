#include <map>

#ifndef AMBIENTE
#define AMBIENTE

#include "banco.hpp"
#include "vars.hpp"

namespace ambiente
{
  // Variáveis globais
  extern vars::cVar global;
  
  // Drivers:
  class driver
  {
    public:
    std::string nome;
    virtual std::string run(std::string)=0;
    ~driver(){}
  };
  
  extern std::map<std::string, driver*> drivers;
  
  class driver_stdout : public driver
  {
    public:
    std::string nome="stdout";
    std::string run(std::string codigo)
    {
      std::cout << codigo << std::endl;
      return codigo;
    }
    driver_stdout(){}
  };
  
  #include "stdlib.h"
  
  class driver_talkin : public driver
  {
    public:
    std::string nome="talkin";
    std::string run(std::string codigo)
    {
      using namespace std;
      using namespace pMatch;
      int pos=0;

      // cout << "codigo: " << codigo << endl;

      // cin >> pos;

      // Encontra o operador de atribuição:
      while(codigo[pos] != '=') pos++;

      std::string var = codigo.substr(0,pos);

      pos++;
      cout  << "codigo: " << codigo.substr(pos,codigo.size()-pos) << endl;
      cExpressao exp = cExpressao(codigo,pos);

      vars::cVar* v;
      if(!(v=local.getVar(var)))
        if(!(v=global.getVar(var)))
          v=&(global.addVar(var,""));

      v->setValor(exp.eval(local,global));

      return v->getValor();

      // string texto;
      
      // // Inserir instrução;
      // arrayClass add = arrayClass("add (\"[!-~ \n\t]*\")inst");
      // if(add.match(codigo,0,true))
      // {
      //   texto = add.var.lInt.front().var.front().lInt.front().word;
      //   banco::addInst(texto);
      //   return texto;
      // }
      // return "Em construcao!";
    }
    driver_talkin(){}
  };
}

#endif
