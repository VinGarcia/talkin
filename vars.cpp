#include "vars.hpp"

using namespace vars;
using namespace std;

void vars::cObject::build(pMatch::tInterpretacao tInt) {

  if(tInt.word.size()) {
    (*this)["$"] = tInt.word;
  }

  for(auto& v : tInt.var) {
    // Ignore variáveis sem nome:
    if(v.nome.length()==0) continue;

    std::string nome;
    if(v.nome[0]=='[') {
      nome = v.nome.substr(1,v.nome.size()-1);
    } else {
      nome = v.nome;
    }

    // Else save it as a new Object:
    (*this)[v.nome] = new cObject(v);
  }
}

vars::cObject::~cObject() {
  for(auto& m : (*this)) {
    if(m.second->type == MAP) {
      delete static_cast<Token<cObject*>*>((TokenBase*)m.second)->val;
    }
  }
}

vars::cObject::cObject() : TokenMap_t() {}

vars::cObject::cObject(pMatch::cVar var) : TokenMap_t() {
  build(var.lInt.front());
}

vars::cObject::cObject(pMatch::tInterpretacao tInt) : TokenMap_t() {
  build(tInt);
}

vars::cObject::cObject(const char* valor) : cObject((std::string)valor) {};
vars::cObject::cObject(const std::string& valor) : TokenMap_t() {
  (*this)["$"] = valor;
}

std::string vars::cObject::str()
{
  Token<TokenMap_t*> token = Token<TokenMap_t*>(this, MAP);
  return packToken::str(&token);
}

