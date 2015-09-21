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
      subvars[v.nome] = cObject(v);
    
    // Caso comece com couchete:
    else
      // Insira na posição indicada pelo numero entre os couchetes:
      subvars[v.nome.substr(1,v.nome.size()-1)]=cObject(v);
  }
}

vars::cObject::~cObject() {}

vars::cObject::cObject() {}

vars::cObject::cObject(pMatch::cVar var)
{
  build(var.lInt.front());
}

vars::cObject::cObject(pMatch::tInterpretacao tInt)
{
  build(tInt);
}

vars::cObject::cObject(const char* valor) : cObject((string)valor) {};
vars::cObject::cObject(string valor)
{
  this->valor = valor;
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
// @param - endereco: endereço da variavel em relação a esta.
//          create: booleano, se `true` novas variaveis serão alocadas
//          quando o endereco referenciar variáveis inexistentes.
//
// @return - Em caso de sucesso retorna a variável desejada ou uma recem alocada.
//         - Em caso de fracasso retorna a variável cObject::undefined.
//
cObject& vars::cObject::child(string endereco, bool create)
{
  if(this->defined) {
    // Lança a versão recursiva dessa função:
    checkAddr(endereco);
    return rec_child(endereco, 0, create);
  }
  else
    throw "vars::cObject::child: undefined or null variable has no child!";
}

cObject& vars::cObject::rec_child(string& endereco, int pos, bool create)
{
  int ini, fim;
  
  // Check for invalid parameters:
  if(endereco==string("")) return null;

  // TODO: Remove this if, it's probably never used
  if(endereco[pos]=='\0')  return null;

  // Drop any leading white spaces:
  while(endereco[pos]==' ' || endereco[pos]=='\t' || endereco[pos]=='\n') pos++;
  
  // Encontre o nome da próxima variável:
  if(endereco[pos]=='[')
  {
    pos++;
    ini = pos;
      
    // Encontre o final do número:
    while(endereco[pos]!=']' && endereco[pos]) pos++;
    if(!endereco[pos]) throw "Endereco de variável abre couchete mas não o fecha! vars::cObject::child()!";
    pos++;
  }
  else
  {
    if(endereco[pos]=='.') pos++;
    ini = pos;
    
    // Encontre o final do nome:
    while(endereco[pos]!='.' && endereco[pos]!='[' && endereco[pos]) pos++;
  
    if(ini==pos) throw "Esperado nome de variável antes de '.', '[' ou '\\0'. vars::cObject::child()";
  }
  
  fim = pos-1;

  // Strip the name from leading and trailing empty spaces:
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

void vars::cObject::checkAddr(std::string& addr) {
  int aux=0;
  static pMatch::charClass check("[^][.a-zA-Z0-9_ \n\t]");
  static pMatch::charClass notBlank("[^ \n\t]");

  // Check if addr has no invalid characters.
  if(check.find(addr, aux))
    throw "Caractere inválido no endereço! Utilize apenas letras, números, '[',']','.' e '_'! vars::cObject::checkAddr()";

  int pos=0, last=0;
  bool begin, end;
  bool insideBrackets = false;
  while(true) {
    // Find the next separator and check for spaces inside names:
    begin = false; end=false;
    while(addr[pos] != '.' && addr[pos] != '[' && addr[pos] !=  ']' && addr[pos] != '\0') {
      if(begin && end && (addr[pos] != ' ' && addr[pos] != '\n' && addr[pos] != '\t'))
        throw "Nomes de variáveis não podem conter espaços em branco! vars::cObject::checkAddr()";
      if(begin && !end && (addr[pos] == ' ' || addr[pos] == '\n' || addr[pos] == '\t'))
        end = true;
      if(!begin && addr[pos] != ' ' && addr[pos] != '\n' && addr[pos] != '\t')
        begin = true;

      pos++;
    }

    last = pos;
    pos++;

    if(addr[last] == '\0') {
      if(insideBrackets)
        throw "Faltou um ']' ao fim do endereço! vars::cObject::checkAddr()";
      else
        break;
    }

    if(addr[last] == '[') {
      if(insideBrackets)
        throw "Caractere '[' não esperado entre couchetes! vars::cObject::checkAddr()";
      else
        insideBrackets = true;
    }

    if(addr[last] == ']') {
      if(insideBrackets) {
        insideBrackets = false;
        char next = notBlank.find(addr, aux=pos);
        if(next != '\0' && next != '.' && next != '[')
          throw "Caractere inesperado após ']'! vars::cObject::checkAddr()";
      }
      else
        throw "Caractere ']' não esperado! vars::cObject::checkAddr()";
    }

    if(addr[last] == '.') {
      if(insideBrackets)
        throw "Caractere '.' não permitido entre couchetes! vars::cObject::checkAddr()";
      else {
        char next = notBlank.find(addr, (aux=pos));
        if(next == '[' || next == '\0')
          throw "Sintaxe inválida: `.[` ou '.' ao fim do endereço! vars::cObject::checkAddr()";
      }
    }
  }
}

std::string vars::cObject::str()
{
  string resp;

  if(!this->defined) return "undefined";

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

