#define CPP
#include "vars.hpp"

using namespace vars;
using namespace std;

// cObject utilizado para denotar
// variaveis não definidas quando utilizando
// o metodo getVar.
cObject vars::cObject::undefined = cObject(true);
cObject vars::cObject::null = cObject(true);

void vars::cObject::build(pMatch::tInterpretacao tInt)
{
  this->valor = tInt.word;

  for(auto& v : tInt.var)
  {
    // Ignore variáveis sem nome:
    if(v.nome.length()==0) continue;

    // Se o nome não começar com couchete:
    if(v.nome[0]!='[' && v.nome.length()>0) 
      subvars[v.nome] = cObject(v, this->deep+1);
    
    // Caso comece com couchete:
    else
      // Insira na posição indicada pelo numero entre os couchetes:
      array[atoi(v.nome.substr(1,v.nome.size()-1).c_str())]=cObject(v, this->deep+1);
  }
}

vars::cObject::~cObject() {}

vars::cObject::cObject() {bBuild=true;}

vars::cObject::cObject(pMatch::cVar var)
{
  build(var.lInt.front());
  bBuild=true;
}

vars::cObject::cObject(pMatch::cVar var, int deep)
{
  this->deep = deep;

  build(var.lInt.front());
  bBuild=true;
}

vars::cObject::cObject(pMatch::tInterpretacao tInt)
{
  build(tInt);
  bBuild=true;
}

vars::cObject::cObject(const char* valor) : cObject((string)valor) {};
vars::cObject::cObject(string valor)
{
  this->valor = valor;
  this->deep = 0;
  bBuild=true;
}

cObject* vars::cObject::operator[](string& endereco)
{
  return &child(endereco);
}

bool vars::cObject::operator==(string& str) {
  if(!this->defined) return false;
  return this->valor == str;
}

bool vars::cObject::operator==(cObject& obj) {
  // If both are undefined return true:
  if(this->defined == false && obj.defined == false) return true;

  // If only one of them is defined return false:
  if(this->defined == false || obj.defined == false) return false;

  // If both are defined check the object value:
  return this->valor == obj.valor;
}

bool vars::cObject::operator==(bool& b) {
  return this->defined == b;
}

vars::cObject::operator bool() {
  return this->defined;
}

//
// @name - child
//
// @desc - Recebe um endereço do tipo: a.b.c...
//         e retorna se existir o valor da variável
//         correspondente.
//
// @param - endereco: hierarquia da variavel em relação a esta.
//          create: booleano, se `true` novas variaveis serão alocadas
//          quando o endereco referenciar variáveis inexistentes.
//
// @return - Em caso de sucesso retorna a variável desejada ou uma recem alocada.
//         - Em caso de fracasso retorna a variável cObject::undefined.
//
cObject& vars::cObject::child(string endereco, bool create)
{
  // Lança a versão recursiva dessa função:
  return rec_child(endereco, 0, create);
}

cObject& vars::cObject::rec_child(string& endereco, int pos, bool create)
{
  int ini, fim;
  
  // Check for invalid parameters:
  if(endereco==string("")) return null;
  if(endereco[pos]=='\0')  return null;
  
  // Encontre o nome da primeira variável:
  if(endereco[pos]=='[')
  {
    pos++;
    ini = pos;
    
    // Encontre o final do numero:
    while(endereco[pos]!=']' && endereco[pos]) pos++;
    if(!endereco[pos]) throw "Endereco de variavel abre couchete mas não o fecha! vars::cObject::child()!";
    pos++;
  }
  else
  {
    if(endereco[pos]=='.') pos++;
    ini = pos;
    
    // Encontre o final do nome:
    while(endereco[pos]!='.' && endereco[pos]!='[' && endereco[pos]) pos++;
    
    if(ini==pos) throw "Esperado nome de variável antes de '.', '[' ou '\0'. vars::cObject::child()";
  }
  
  fim = pos-1;

  // Strip the name from empty spaces:
  while(endereco[ini]==' ' || endereco[ini]=='\t' || endereco[ini]=='\n') ini++;
  while(endereco[fim]==' ' || endereco[fim]=='\t' || endereco[fim]=='\n') fim--;
  
  // Extract the name:
  string var_name = endereco.substr(ini, fim-ini+1);

  // Caso o endereço tenha terminado em pos.
  if(endereco[pos]=='\0')
  {
    // Verifique se existe uma subvar compatível e retorne seu valor.
    if(subvars.count(var_name)==1) return subvars[var_name];

    if(create == true) {
      subvars[var_name] = cObject();
      return subvars[var_name];
    }
    
    // Caso não haja uma subvar compatível retorne false:
    return undefined;
  }
  
  // Caso contrário e se existir uma próxima variável, continue a recursão:
  if(subvars.count(var_name)==1)
    return subvars[var_name].rec_child(endereco, pos, create);

  if(create == true) {
    subvars[var_name] = cObject();
    return subvars[var_name].rec_child(endereco, pos, create);
  } 

  return undefined;
}

std::string vars::cObject::str()
{
  string resp;

  if(this->valor != "")
    resp += "{$:'" + this->valor +"',";
  else
    resp += "{";

  if(subvars.size()>0)
  {
    for(auto& i : subvars) {
      resp += i.first + ":";
      resp+=i.second.str();
      resp += ",";
    }
  }

  if(resp.length()!=1)
    resp.pop_back();

  resp += "}";

  return resp;
}

