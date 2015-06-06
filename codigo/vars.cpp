#define CPP
#include "vars.hpp"

using namespace vars;
using namespace std;

// cVar utilizado para denotar
// variaveis não definidas quando utilizando
// o metodo getVar.
cVar vars::cVar::undefined = cVar(true);

void vars::cVar::build(pMatch::tInterpretacao tInt)
{
  cVar* grand_father;
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
      subvars[v.nome] = cVar(v, grand_father,this->deep+1);
    
    // Caso comece com couchete:
    else
      // Insira na posição indicada pelo numero entre os couchetes:
      array[atoi(v.nome.substr(1,v.nome.size()-1).c_str())]=cVar(v, grand_father, this->deep+1);
  }
}

vars::cVar::~cVar() {}

vars::cVar::cVar() {bBuild=true;}

vars::cVar::cVar(pMatch::cVar var)
{
  this->nome = var.nome;

  build(var.lInt.front());
  bBuild=true;
}

vars::cVar::cVar(pMatch::cVar var, cVar* grand_father, int deep)
{
  this->nome = var.nome;
  this->grand_father = grand_father;
  this->deep = deep;

  build(var.lInt.front());
  bBuild=true;
}

vars::cVar::cVar(string nome, pMatch::tInterpretacao tInt)
{
  this->nome = nome;
  
  build(tInt);
  bBuild=true;
}

vars::cVar::cVar(string nome, string valor)
{
  this->nome = nome;
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
cVar* vars::cVar::getVar(string endereco)
{
  cVar* grand_father = this->grand_father;
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

cVar* vars::cVar::operator[](string endereco)
{
  return getVar(endereco);
}

void vars::cVar::addChild(string nome, string valor)
{
  subvars[nome] = cVar(nome,valor);
  subvars[nome].deep = this->deep+1;

  if(!grand_father)
    subvars[nome].grand_father = this;
  else
    subvars[nome].grand_father = this->grand_father;
}

void vars::cVar::addChild(cVar child)
{
  this->subvars[child.nome] = cVar(child.nome,child.valor);
  subvars[child.nome].deep = this->deep+1;

  if(!grand_father)
    subvars[child.nome].grand_father = this;
  else
    subvars[child.nome].grand_father = this->grand_father;
}
cVar& vars::cVar::addVar(std::string addr, std::string valor)
{
  // Pule os espaços em branco no inicio do nome:
  int ini=0;
  while(addr[ini]==' ' || addr[ini]=='\n' || addr[ini]=='\t') ini++;
  int fim=addr.length()-1;
  while(addr[fim]==' ' || addr[fim]=='\n' || addr[fim]=='\t') fim--;

  return addVar(addr.substr(ini,fim-ini+1),valor,NULL);
}

/*
 * @nome - addVar
 * 
 * @desc - recebe um endereço de variável e um valor.
 *         e adiciona caso não existam todas as variáveis
 *         descritas no endereço como subvariaveis
 *         uma das outras até chegar na última variável do endereço.
 *         Essa última variável além de ser criada tambeḿ recebe o valor citado.
 *
 * @param - endereco_var: endereço a ser criado.
 *          valor: valor a ser adicionado à última variável criada.
 *          nearestVar: ponteiro usado internamente para evitar
 *            um loop desnecessário.
 *            Caso o valor seja null o loop é realizado
 *            e o valor obtido é escrito em nearestVar.
 *
 */
cVar& vars::cVar::addVar(std::string endereco_var, std::string valor, cVar* nearestVar)
{
  int i=0, inicio;
  string nome;
  cVar var;

  if(endereco_var==string(""))
    throw "endereco_var é uma string vazia!:vars::cVar::addVar()";

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
    throw "Valor da variável privada deep corrompido!:vars::cVar::addVar()";
  
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

cVar& vars::cVar::setValor(std::string valor)
{
  cVar* resp=this;
  if(this == &undefined)
  {
    if(grand_father->undef_father==NULL)
      throw "undef_father corrompido!:vars::cVar::operator=()";
    
    resp = &(grand_father->addVar(grand_father->undef_addr,""));
  }

  resp->valor = valor;

  return (*resp);
}

std::pair<bool,cVar&> vars::cVar::getNearestVar(string endereco)
{
  // Lança a versão recursiva dessa função:
  return rec_getNearestVar(endereco,0);
}

std::pair<bool,cVar&> vars::cVar::rec_getNearestVar(string& endereco, int pos)
{
  int ini, fim;
  
  if(endereco==string("")) return {false,*this};
  if(endereco[pos]=='\0')  return {false,*this};
  
  // Encontre o nome da primeira variável:
  if(endereco[pos]=='[')
  {
    pos++;
    ini = pos;
    
    // Encontre o final do numero:
    while(endereco[pos]!=']' && endereco[pos]) pos++;
    if(!endereco[pos]) throw "Endereco de variavel abre couchete mas não o fecha! vars::cVar::getNearestVar()!";
    pos++;
  }
  else
  {
    if(endereco[pos]=='.') pos++;
    ini = pos;
    
    // Encontre o final do nome:
    while(endereco[pos]!='.' && endereco[pos]!='[' && endereco[pos]) pos++;
    
    if(ini==pos) throw "Esperado nome de variável antes de '.', '[' ou '\0'. vars::cVar::getNearestVar()";
  }
  
  // Desconsidere espaços em branco ao inicio:
  while(endereco[ini]==' ' || endereco[ini]=='\t' || endereco[ini]=='\n') ini++;
  
  fim = pos-1;
  // Desconsidere espaços em branco ao fim:
  while(endereco[fim]==' ' || endereco[fim]=='\t' || endereco[fim]=='\n') fim--;
  
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

std::string vars::cVar::str()
{
  string resp = "(" + this->nome + ":" + this->valor + ")";
  resp += to_string(this->deep);

  if(subvars.size()>0)
  {
    resp += " {";
    for(auto& i : subvars)
      resp+=i.second.str()+",";
    resp.pop_back();
    resp += "}";
  }

  return resp;
}

