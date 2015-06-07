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
  cObject* grand_father;
  if(this->grand_father == NULL)
    grand_father = this;
  else
    grand_father = this->grand_father;

  this->valor = tInt.word;

  for(auto& v : tInt.var)
  {
    // Ignore variáveis sem nome:
    if(v.nome.length()==0) continue;

    // Se o nome não começar com couchete:
    if(v.nome[0]!='[' && v.nome.length()>0) 
      subvars[v.nome] = cObject(v, grand_father,this->deep+1);
    
    // Caso comece com couchete:
    else
      // Insira na posição indicada pelo numero entre os couchetes:
      array[atoi(v.nome.substr(1,v.nome.size()-1).c_str())]=cObject(v, grand_father, this->deep+1);
  }
}

vars::cObject::~cObject() {}

vars::cObject::cObject() {bBuild=true;}

vars::cObject::cObject(pMatch::cVar var)
{
  build(var.lInt.front());
  bBuild=true;
}

vars::cObject::cObject(pMatch::cVar var, cObject* grand_father, int deep)
{
  this->grand_father = grand_father;
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

//
// @name - getVar
//
// @desc - Recebe um endereço do tipo: a.b[10].c
//         e retorna se existir o valor da variável
//         correspondente entre suas subvars.
//
// @return - Em caso de sucesso retorna a variável em questão.
//         - Em caso de fracasso retorna a variável estatica 'undefined'
//
cObject* vars::cObject::getVar(string endereco)
{
  cObject* grand_father = this->grand_father;
  if(this->grand_father==NULL)
    grand_father=this;

  auto nearest = getNearestVar(endereco);
  grand_father->undef_father = &nearest.second;
  grand_father->undef_addr = endereco;

  // Caso a variável mais próxima seja a 'nearest' retorne-a:
  if(nearest.first==true) return &(nearest.second);
  // caso contrário retorne 'undefined':
  else return NULL;
}

cObject* vars::cObject::operator[](string endereco)
{
  return getVar(endereco);
}

void vars::cObject::addChild(string nome, const char* valor) {
  addChild(nome, string(valor));
}
void vars::cObject::addChild(string nome, string valor)
{
  subvars[nome] = cObject(valor);
  subvars[nome].deep = this->deep+1;

  if(!grand_father)
    subvars[nome].grand_father = this;
  else
    subvars[nome].grand_father = this->grand_father;
}

void vars::cObject::addChild(string nome, cObject child)
{
  this->subvars[nome] = cObject(child.valor);
  this->subvars[nome].deep = this->deep+1;

  if(!grand_father)
    subvars[nome].grand_father = this;
  else
    subvars[nome].grand_father = this->grand_father;
}
cObject& vars::cObject::addVar(std::string addr, std::string valor)
{
  int ini=0;
  int fim=addr.length()-1;

  // Strip the name from empty spaces:
  while(addr[ini]==' ' || addr[ini]=='\n' || addr[ini]=='\t') ini++;
  while(addr[fim]==' ' || addr[fim]=='\n' || addr[fim]=='\t') fim--;

  return addVar(addr.substr(ini,fim-ini+1),valor,NULL);
}

/*
 * @nome - addVar
 * 
 * @desc - recebe um endereço de variável e um valor.
 *         Se o endereço da forma a.b.c contiver variáveis não
 *         declaradas aloque cada uma delas.
 *         A última variável além de ser alocada receberá o valor como atributo.
 *
 * @param - endereco_var: endereço completo da variável.
 *          valor: valor a ser adicionado à última variável criada.
 *          nearestVar: ponteiro usado internamente para evitar
 *            um loop desnecessário.
 *            Caso o valor seja null o loop é realizado
 *            e o valor obtido é escrito em nearestVar.
 *
 */
cObject& vars::cObject::addVar(std::string endereco_var, std::string valor, cObject* nearestVar)
{
  int i=0, inicio;
  string nome;
  cObject var;

  if(endereco_var==string(""))
    throw "endereco_var é uma string vazia!:vars::cObject::addVar()";

  if(nearestVar==NULL)
    nearestVar=&(getNearestVar(endereco_var).second);

  int deep = nearestVar->deep - this->deep;

  // Encontre no endereço onde começa
  // a parte "undefined" ou seja que precisa ser adicionada.
  for(i=0;deep>0;deep--)
  {
    // Encontre o próximo '.' ou '[':
    while(endereco_var[i]!='.' && endereco_var[i]!='[' && endereco_var[i]) i++;
  }
  
  if(!endereco_var[i])
    throw "Valor da variável privada deep corrompido!:vars::cObject::addVar()";
  
  // Aponte para o inicio do nome:
  if(endereco_var[i]=='.') i++;
  // Note que para variáveis em arrays o nome fica na forma:
  // nome = "[0]" ou nome = "[10]"

  inicio=i;

  // Encontre o fim do nome:
  while(endereco_var[i]!='.' && endereco_var[i]!='[' && endereco_var[i]) i++;

  nome = endereco_var.substr(inicio,i-inicio);
  
  // Se o endereço não tiver acabado, continue a recursão:
  if(endereco_var[i] != '\0')
  {
    // Faça o i apontar para a próxima posição:
    i++;
    // E adicione a variável cujo nome já foi encontrado com valor vazio:
    nearestVar->addChild(nome,"");
    return nearestVar->subvars[nome].addVar(endereco_var.substr(i,endereco_var.length()-i),valor,&(nearestVar->subvars[nome]));
  }
  else
  {
    // Senão crie uma nova variavel com o valor = 'valor'.
    // Adicione a variável às subvars da nearest:
    nearestVar->addChild(nome,valor);
    // E retorne:
    return nearestVar->subvars[nome];
  }
}

cObject& vars::cObject::setValor(std::string valor)
{
  cObject* resp=this;
  if(this == &undefined)
  {
    if(grand_father->undef_father==NULL)
      throw "undef_father corrompido!:vars::cObject::operator=()";
    
    resp = &(grand_father->addVar(grand_father->undef_addr,""));
  }

  resp->valor = valor;

  return (*resp);
}

std::pair<bool,cObject&> vars::cObject::getNearestVar(string endereco, bool create)
{
  // Lança a versão recursiva dessa função:
  return rec_getNearestVar(endereco, 0, create);
}

std::pair<bool,cObject&> vars::cObject::rec_getNearestVar(string& endereco, int pos, bool create)
{
  int ini, fim;
  
  // Check for invalid parameters:
  if(endereco==string("")) return {false,*this};
  if(endereco[pos]=='\0')  return {false,*this};
  
  // Encontre o nome da primeira variável:
  if(endereco[pos]=='[')
  {
    pos++;
    ini = pos;
    
    // Encontre o final do numero:
    while(endereco[pos]!=']' && endereco[pos]) pos++;
    if(!endereco[pos]) throw "Endereco de variavel abre couchete mas não o fecha! vars::cObject::getNearestVar()!";
    pos++;
  }
  else
  {
    if(endereco[pos]=='.') pos++;
    ini = pos;
    
    // Encontre o final do nome:
    while(endereco[pos]!='.' && endereco[pos]!='[' && endereco[pos]) pos++;
    
    if(ini==pos) throw "Esperado nome de variável antes de '.', '[' ou '\0'. vars::cObject::getNearestVar()";
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
    if(subvars.count(var_name)==1) return {true,subvars[var_name]};
    
    // Caso não haja uma subvar compatível retorne false:
    return {false,*this};
  }
  
  // Caso contrário, se existir uma próxima variável, continue a recursão:
  if(subvars.count(var_name)==1)
    return subvars[var_name].rec_getNearestVar(endereco, pos);
  else return {false,*this};
}

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
    if(!endereco[pos]) throw "Endereco de variavel abre couchete mas não o fecha! vars::cObject::getNearestVar()!";
    pos++;
  }
  else
  {
    if(endereco[pos]=='.') pos++;
    ini = pos;
    
    // Encontre o final do nome:
    while(endereco[pos]!='.' && endereco[pos]!='[' && endereco[pos]) pos++;
    
    if(ini==pos) throw "Esperado nome de variável antes de '.', '[' ou '\0'. vars::cObject::getNearestVar()";
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
    return subvars[var_name].rec_child(endereco, pos);

  if(create == true) {
    subvars[var_name] = cObject();
    return subvars[var_name].rec_child(endereco, pos);
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

