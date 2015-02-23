#include "banco.hpp"
#include "ambiente.hpp"
#include <stdlib.h>
#include <sstream>

using namespace std;
using namespace pMatch;

vars::cVar local;

// charClasses utilizados para lidar com caracteres em branco:
charClass blank = charClass("[ \t\n]");
charClass notBlank = charClass("[^ \t\n]");

// Last id é o último ID adicionado no banco.
// Valor inicial = 0.
int banco::lastID=0;

// O nível default de adição de novas instruções.
// O padrão é 1, mas isso pode mudar.
int banco::def_level=1;

// TODO: tirar essa limitação do vetor de níveis.
// Vetor de níveis atualmente limitado ao nível [10]
vector<map<int, cInst>> banco::instrucoes = vector<map<int,cInst>>(11);

std::map<std::string, cRotulo> banco::rotulos;

/* * * * * START banco * * * * */

cInst* banco::addInst(string inst, int level)
{
  int ID = getUniqueID();
  instrucoes[level][ID] = cInst(inst, ID);
  return &(instrucoes[level][ID]);
}

cInst* banco::addInst(string inst)
{
  int ID = getUniqueID();
  instrucoes[def_level][ID] = cInst(inst, ID);
  return &(instrucoes[def_level][ID]);
}

pair<cInst*, int> banco::getInst(int ID)
{
  int i;
  bool found=false;
  for(i=def_level; i>=0; i--)
    if(instrucoes[i].count(ID))
    {
      found=true;
      break;
    }
  
  if(found) return {&(instrucoes[i][ID]),i};
  else return {NULL,0};
}

void banco::remInst(int ID)
{
  auto inst = getInst(ID);
  
  if(!inst.first) return;
  else instrucoes[inst.second].erase(ID);
}

// Usada para fins de facilitar testes.
void banco::remLast()
{
  remInst(lastID);
}

// Retorna as instruções do banco em uma única string.
string banco::str()
{
  std::string str;
  for(int i=0; (unsigned)i<instrucoes.size(); i++)
  {
    for(auto j=instrucoes[i].begin(); j!=instrucoes[i].end(); j++)
    {
      std::ostringstream oss;
      oss <<"inst["<< i <<"]["<< j->first <<"]: «"<<j->second.str() << "»\n";
      str += oss.str();
    }
  }
  // Remove o ultimo '\n':
  str.pop_back();
  return str;
}

// Retorna as instruções do banco em uma lista de strings.
list<string> banco::strList()
{
  std::list<std::string> list;
  for(int i=0; (unsigned)i<instrucoes.size(); i++)
  {
    for(auto j=instrucoes[i].begin(); j!=instrucoes[i].end(); j++)
    {
      std::ostringstream oss;
      oss <<"inst["<< i <<"]["<< j->first <<"]: «"<<j->second.str() << "»";
      list.push_back(oss.str());
    }
  }
  return list;
}

// Retorna a lista de rotulos em formato de string de lista.
list<string> banco::rotList()
{
  list<string> resp;
  
  for(auto& rot : rotulos)
  {
    resp.push_back(rot.second.str());
  }
  return resp;
}

// O match verifica se o texto recebido em str é compatível com o texto
// do padrão da instrução. Retornando true caso positivo.
// 
// Adicionalmente verifica-se o contexto atual, acessando-se as variáveis globais e locais.
// Caso o contexto seja falso segundo as variáveis globais e locais atuais a função retorna false.
bool cInst::match(string str)
{
  // Antes de qualquer coisa, uma instrução só pode receber match
  // diretamente se ela tiver um significado.
  // Caso contrário a instrução somente delimita um rótulo e portanto não é acessada por aqui.
  if(significadoOmitido) return false;
  
  int num_validos=0;
  bool bMatch = this->padrao.match(str, 0, true);
  this->var = padrao.var;
  
  if(!bMatch) return false;
  
  // Para cada interpretação possível, verifique se o contexto é válido:
  for(auto j=var.lInt.begin(); j!= var.lInt.end(); )
  {
    auto v = vars::cVar("local",*j);
    // Se o contexto não for válido:
    //if(contexto.eval(vars::cVar(j->var),ambiente::global))
    if(!contexto.eval(v,ambiente::global))
    {
      j=var.lInt.erase(j);
      continue;
    }
    
    // TODO: verificar como que se retorna os matchs.
    // Porque aqui não há distinção entre os que tem contexto válido e os demais.
    num_validos++;
    j++;
  }
  
  if(num_validos==0) return false;
  else return true;
}

std::string cInst::str()
{
  using namespace std;
  string resp = string("");
  list<string>::iterator str_it;
  list<cSignificado>::iterator sig_it;
  
  // Adiciona os rotulos na resposta:
  if(!lRotulos.empty())
  {
    for(str_it=lRotulos.begin(); str_it!=lRotulos.end(); str_it++)
      resp+=(*str_it) + ", ";
    resp[resp.length()-2] = ':';
  }
  
  // Adiciona o padrao na resposta:
  resp += padrao.str();

  // Adiciona o contexto na resposta:
  if(!contexto.empty())
  {
    resp += " - ";
    resp += contexto.str();
  }
  
  if(!lSignificado.empty())
  {
    resp += " => ";
    for(sig_it=lSignificado.begin(); sig_it!=lSignificado.end(); sig_it++)
      resp += sig_it->str() + ' ';

    // Remove o ultimo espaço:
    resp.pop_back();
  }
  
  return resp;
}


// TODO: mover essa função para algum lugar adequado.
// Essa função transforma uma lista de variáveis em uma string.
string lVar_to_str(pMatch::lVar vars)
{
  if(vars.size()==0) return string("");
  
  string str = string("");
  
  for(auto v : vars)
    str += v.str() + ", ";
  
  // Remove a última virgula e espaço:
  str.pop_back();
  str.pop_back();
  
  return str;
}

void banco::execInst(string texto)
{
  bool bMatch=false;
  
  vector<cInst> inst;
  vector<pMatch::cVar> escopo;
  
  // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  // Casamento de padrao:

  // Encontra as interpretações possíveis:
  for(int level = def_level; level>=0 && bMatch==false; level--)
  {
    for(auto& i : instrucoes[level])
    {
      // Se houver match:
      if(i.second.match(texto))
      {
        // Marque que houve match:
        bMatch = true;
        // Guarde a instrução que recebeu o match:
        inst.push_back(i.second);
        // E colete a variável associada (que contem as interpretacoes):
        escopo.push_back(i.second.var);
        // Incremente o contador de matchs:
  // cout << "instrucao: " << i.second.str() << endl;
      }
    }
  }

  // Caso não tenha-se encontrado nem uma interpretação:
  if(inst.size()==0)
    throw "Nao foi possivel interpretar o texto!: cInst::execInst()";

  // cout << "escopo inicial: " << escopo.front().str() << endl;
  
  // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  // Tratamento de ambiguidades:
  
  int j;
  pMatch::tInterpretacao escopo_escolhido;
  cInst inst_escolhida;

  // Se houver mais de uma interpretação possível:
  if(inst.size()>1 || escopo[0].lInt.size()>1)
  // Trata as ambiguidades perguntando ao usuário qual delas é a certa.
  {
    // Aqui será mapeada a resposta do usuário de um inteiro
    // para um uma instancia de {pMatch::tInterpretacao, cInst}
    map<int, pair<pMatch::tInterpretacao*, cInst*>> index;
    
    j=0;
    int escolha=0;
    cout << "Ambiguidades encontradas: " << endl << endl;
    
    for(j=0; (unsigned)j<inst.size(); j++)
    {
      if(escopo[j].lInt.size()==1 && escopo[j].lInt.front().var.size()==0)
      {
        index[escolha] = {&escopo[j].lInt.front(), &(inst[j])};
        cout << "    " << escolha++ << " »» " << inst[j].str() << endl;
      }
      else
      {
        cout << "  " << inst[j].str() << endl;
        
        for(auto& k : escopo[j].lInt)
        {
          index[escolha] = {&k, &(inst[j])};
          cout << "    " << escolha++ << " »» " << lVar_to_str(k.var) << endl;
        }
      }
      cout << endl;
    }
    
    string line;
    
    // Agora aguardamos a resposta do usuário:
    getline(cin, line);
    
    escolha = atoi(line.c_str());
    
    // E guardamos a interpretação escolhida por ele.
    escopo_escolhido = *(index[escolha].first);
    inst_escolhida = *(index[escolha].second);
  }
  else
  {
    escopo_escolhido = escopo[0].lInt.front();
    inst_escolhida = inst[0];
  }
  
  // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  // Execução:
  
  executa(inst_escolhida, escopo_escolhido);
}

/* * * * * END banco * * * * */

bool cRotulo::match(string str, int pos)
{
  // Limpa a lista antiga:
  this->var.lInt.clear();
  this->match_word.clear();
  
  int level = banco::getLevel();
  bool bMatch = false;
  pMatch::arrayClass* aux;
  
  // Note que procura-se do nível mais alto para o mais baixo.
  // Se houver match em um nível os níveis inferiores são ignorados.
  for(; level>=0 && !bMatch; level--)
    // Para cada referencia à instrução:
    for(auto& instID : this->instID)
    {
      // Se a instrução instID não estiver nesse level:
      if(!banco::instrucoes[level].count(instID)) continue;
      
      // Caso contrário encontre e salve em aux o padrão da instrução:
      aux = &(banco::instrucoes[level][instID].padrao);
      
      // Se houver match, salve os valores do match, e continue.
      if(aux->match(str, pos))
      {
        bMatch=true;
        this->var.lInt.splice(this->var.lInt.end(), aux->var.lInt);
        this->match_word.splice(this->match_word.end(), aux->getMatch());
      }
    }
  
  return bMatch;
}

/* * * * * START cInst * * * * */
  
// TODO: Validar o formato:
// significado: #!driver: "texto";
// validar o formato das variáveis no contexto (isso é invalido: "con.[0]")
// validar o formato das variáveis no significado (mesmo tipo de problema)
  void cInst::validate(string inst)
  {
    int pos=1;
    int antes;
    
    if(mRotulo.match(inst,0))
      throw "não pode começar a inst com ':,-': cInst::cInst()";
    
    // Loop do rotulo:
    while((unsigned)pos<inst.length())
    {
      antes=pos;
      
      while((unsigned)pos<inst.length())
      {
        // Encontre um dos caracteres de fim:
        mRotulo.find(inst,pos);
        
        // Se o caractere não estava precedido por uma '\':
        if(inst[pos-1] != '\\' || !inst[pos])
        {
          if(inst[pos]=='=' && inst[pos+1]!='>') break;
          else break;
        }
        
        // Caso contrário pule o caractere e continue:
        pos++;
      }

      // Se encontrou um '\0' a instrução está mal formatada:
      if(!inst[pos])
        throw "Instrução inválida - sem separadores: cInst::cInst()";
      
      // Se encontrou um '=>':
      if(inst[pos] == '=')
      {
        rotuloOmitido=true;
        contextoOmitido=true;
        pos=0;
        break;
      }
      
      // Se encontrar um '-' antes do ':'
      // Significa que o campo de rotulo foi omitido.
      if(inst[pos] == '-')
      {
        // Então zere a busca:
        pos=0;
        // Ative o flag:
        rotuloOmitido=true;
        // E termine o while do rotulo:
        break;
      }
      
      // Verifique se o intervalo coberto não está em branco:
      notBlank.find(inst, antes);
      if(antes==pos)
        throw "Rotulo vazio na instrução! cInst::cInst()";
      
      if(inst[pos]==':')
      {
        // Anote a posição do ':':
        fimRotulo=pos;
        
        // Salte o ':'
        pos++;
        break;
      }
      
      // ... Se for uma virgula não faça nada ...
      
      // Incremento pos para na próxima iteração ele iniciar
      // a busca a partir do próximo caractere:
      pos++;
    }
    
    // Loop do padrão:
    int i=1;
    bool bClass = false;
    int posClass;
    while((unsigned)pos<inst.length() && i-- > 0)
    {
      // Encontre o próximo caractere de fim do padrão:
      while((unsigned)pos<inst.length())
      {
        if(inst[pos-1]!='\\' || !inst[pos])
        {
          if(inst[pos] == '[') { bClass=true; posClass=pos; }
          
          if(!bClass && inst[pos] == '-') break;
          
          // Se for a setinha '=>'
          if(inst[pos]=='=' && inst[pos+1] == '>') break;
          
          if(!inst[pos]) break;
        }
        
        // (o caractere de fim de classe não pode ser escapado por um '\\':
        if(    bClass
            && inst[pos] == ']'
            && !(pos-posClass==1 || (pos-posClass==2 && inst[pos-1]=='^'))
          )
          bClass=false;
        
        pos++;
      }
      
      if(inst[pos] == '\0')
      {
        if(rotuloOmitido)
          throw "Instrução inválida - sem rotulo e sem significado: cInst::cInst()";
        contextoOmitido=true;
        significadoOmitido=true;
        break;
      }
      
      if(inst[pos] == '-')
      {
        // Agora anoto a posição de fim:
        fimPadrao=pos;
        
        // Salte o separador:
        pos++;
        break;
      }
      
      if(inst[pos] == '=')
      {
        // Marco que o contexto foi omitido:
        contextoOmitido=true;
        
        // Anoto a posição de fim do padrão:
        fimPadrao=pos;
        
        // E encerro o loop:
        pos+=2;
        break;
      }
      
      pos++;
    }
    
// TODO: verificar se ha algo no loop abaixo que deve ser mantido ou não.

// Um loop provisório foi criado:
    if((unsigned)pos<inst.size() && !contextoOmitido)
    {
      while(!(inst[pos]=='=' && inst[pos+1]=='>') && inst[pos]) pos++;
      if(inst[pos]=='=')
      {
        fimContexto = pos;
        pos += 2;
      }
      else
        significadoOmitido=true;
    }
  /*
    // Loop do contexto:
    while((unsigned)pos < inst.length() && !contextoOmitido)
    {
      antes=pos;
      
      // Encontro o próximo caractere delimitador:
      while((unsigned)pos<inst.length())
      {
        // Encontre um dos caracteres delimitadores:
        mContexto.find(inst,pos);
        
        if(inst[pos-1] != '\\' || !inst[pos])
        {
          if(inst[pos]=='=' && inst[pos+1] == '>') break;
          if(!inst[pos] || inst[pos]==',') break;
          else
            throw "Caractere inválido no contexto!, use apenas nomes de variáveis, '.' e ','!: cInst::cInst()";
        }
        
        // Caso contrário pule o caractere e continue:
        pos++;
      }
      
      // Verifique se não há um item em branco na lista de contextos:
      notBlank.find(inst, antes);
      if(antes==pos)
        throw "item em branco no Contexto!: cInst::cInst()";
      
      if(inst[pos] == '\0')
      {
        significadoOmitido=true;
        break;
      }
      
      if(inst[pos] == '=')
      {
        // Anoto a posição de fim do contexto:
        fimContexto = pos;
        
        // E encerro o loop:
        pos+=2;
        break;
      }
      
      pos++;
    }
  // */
    
    // Loop do significado:
    while((unsigned)pos < inst.length() && !significadoOmitido)
    {
      bool variavel=false;
      antes=pos;
      
      while((unsigned)pos < inst.length())
      {
        // Procuro os caracteres delimitadores do significado:
        if(!variavel && inst[pos]=='@' && inst[pos-1]!='\\') variavel=true;
        
        if(!variavel && inst[pos]==';' && inst[pos-1]!='\\') break;
        
        if(variavel && inst[pos]==';') variavel=false;
        
        pos++;
      }
      
      if(!inst[pos]) break;
      
      notBlank.find(inst, antes);
      if(antes==pos)
        throw "Item em branco no significado!: cInst::cInst()";
      
      pos++;
    }
    
    this->validated = true;
  }
  
  void cInst::buildRotulo(string rotulo)
  {
    int pos1, pos2;
    bool lastBlank=false;
    string str;
    
    for(pos1=0; (unsigned)pos1<rotulo.length(); pos1++)
    {
      // Descarte os caracteres em branco ao inicio:
      notBlank.find(rotulo, pos1);
      
      // Encontre o próximo separador do rotulo:
      mRotulo.find(rotulo, pos2=pos1);
      
      str = string("");
      
      // Coleto o nome do rotulo removendo espaços repetidos:
      for(; pos1<pos2; pos1++)
      {
        if(!notBlank.match(rotulo[pos1]))
        {
          if(!lastBlank) str += ' ';
          lastBlank=true;
        }
        else
        {
          lastBlank=false;
          str += rotulo[pos1];
        }
      }
      // Se o nome acabava em espaço remova o espaço do fim da string:
      if(rotulo[pos2-1]==' ') str.pop_back();
      
      // Associo essa instrução ao rotulo correspondente na lista de rotulos:
      if(!banco::rotulos.count(str))
        banco::rotulos[str] = cRotulo(str);
      banco::rotulos[str].addInst(this->ID);
// TODO: Pensar um jeito de fazer uma referencia mais explicita ao banco.
//       Acionar diretamente a variável rótulo é implicito e pode
//       causar dificuldade na hora de debugar ou fazer manutenção.
      
      // Anota nesta instrução o nome do novo rotulo:
      this->lRotulos.push_back(str);
    }
  }
  
  void cInst::buildPadrao(string padrao)
  {
    int fim, ini;
    
    // Descarte os espaços no inicio:
    notBlank.find(padrao,ini=0);
    
    // Descarte os espaços no final:
    for(fim=padrao.length()-1; !notBlank.match(padrao[fim]); fim--);
    
    if(padrao[ini]!='"')
      this->padrao = pMatch::arrayClass(
        padrao.substr(ini,padrao.length()-ini-(padrao.length()-(fim+1)))
      );
    else
      // Caso comece com aspas já está no formato adequado para o parser
      // do array class, então é só chama-lo:
      this->padrao = pMatch::arrayClass(padrao,ini);
  }
  
  // TODO: Apagar a função abaixo, não mais utilizada.
/*
  void cInst::buildContexto(string contexto)
  {
    int i;
    int fpos;
    string str=string("");
    
    // Encontre o fim do último contexto.
    for(fpos=contexto.length()-1; !notBlank.match(contexto[fpos]); fpos--);
    fpos++;
    
    // Note que o loop abaixo roda uma última vez para i==fpos
    for(i=0; i<fpos+1; i++)
    {
      // Ignore espaços em branco:
      if(contexto[i]==' ' && i!=fpos) continue;
      
      // Se chegar ao fim de uma expressão de contexto:
      if(contexto[i]==',' || i==fpos)
      {
        // Anoto o contexto:
        this->lContexto.push_back(str);
        // Zero a string:
        str=string("");
        // Continuo a execução:
        continue;
      }
      str+=contexto[i];
    }
  }
*/
  
/*
  cSignificado::build(string significado)
  {
    
  }
// */
  
  cSignificado::cSignificado(const char* str, int& pos):
  cSignificado(string(str), pos){}
  cSignificado::cSignificado(string significado, int& pos)
  {
    int i, ini;
    string str;
    int fimAspas, fimTexto;
    bool aspas=false;
    
    // Encontra o primeiro caractere não branco:
    notBlank.find(significado,i=pos);
    
    // Se houver uma declaração de driver:
    if(significado[i]=='#' && significado[i+1]=='!')
    {
      // Encontra o inicio do nome do driver:
      notBlank.find(significado, i+=2); ini=i;
      
      // Encontra o fim do nome do driver:
      while(notBlank.match(significado, i) && significado[i]!=':') i++;
      
      // Grava o driver na classe:
      this->driver = significado.substr(ini, i-(ini));
      
      // Encontra o ':':
      if(significado[i]!=':')
        notBlank.find(significado, ++i);
      
      // Encontra o inicio do texto:
      notBlank.find(significado, ++i);
    }
    
    // Se o significado começar com aspas:
    if(significado[i]=='"')
    {
      aspas=true;
      for(fimAspas=significado.size()-1; significado[fimAspas]!='"';fimAspas--);
      i++;
    }
    else
    {
      // Encontre a última letra do texto desconsiderando o ';'
      for(fimTexto=significado.size()-1; ; fimTexto--)
        // Se houver uma letra não-branca:
        if( notBlank.match(significado[fimTexto]) )
        {
          // Se for um ';' não precedido por '\\'
          // (isso faria dele um caractere comum): continue,
          if(significado[fimTexto]==';' && significado[fimTexto-1]!='\\');
          // Caso contrário já foi encontrado o último caractere então break;
          else break;
        }
    }
    
    // Leia o texto:
    for(int j=0; (unsigned)i<significado.length()+1; i++)
    {
      if(significado[i]=='@' && significado[i-1]!='\\')
      {
        string var = string("");
        for(i++; significado[i]!=';'; i++)
        {
          // Ignore quaisquer espaços em branco:
          if(!notBlank.match(significado[i])) continue;
          // Preencha o nome da variável:
          var += significado[i];
        }
        
        // Adicione o nome da variável e sua posição no this->variaveis;
        this->variaveis.push_back({var, j});
        continue;
      }
      
      if(aspas && i==fimAspas)
      {
        // Pule a aspas:
        i++;
        // Encontre o ';' final (se houver):
        notBlank.find(significado, i);
        // E quebre o loop:
        break;
      }
      else if(i==fimTexto+1)
      {
        // Encontre o ';' final (se houver):
        notBlank.find(significado, i);
        // E quebre o loop:
        break;
      }
      
      if(!aspas)
        if((significado[i]==';'&&significado[i-1]!='\\')||significado[i]=='\0')
          break;
      
      this->texto += significado[i];
      j++;
    }
    
    if(significado[i]==';') i++;
    pos=i;
    
    // Encerre a construção.
  }
  
  void cInst::buildSignificado(string significado)
  {
    int i;
    for(i=0; (unsigned)i<significado.length(); )
    {
      // Caso não hajam mais letras até o fim da string:
      if(notBlank.find(significado,i)=='\0') break;
      lSignificado.push_back(cSignificado(significado, i));
    }
  }
  
  void cInst::build(string inst)
  {
    int pos;
    
    if(validated)
    {
      if(!rotuloOmitido)
        buildRotulo(inst.substr(0,fimRotulo));
      
      if(rotuloOmitido)
        buildPadrao(inst.substr(0, fimPadrao));
      else
        buildPadrao(inst.substr(fimRotulo+1, fimPadrao-(fimRotulo+1)));
      
      if(!contextoOmitido)
        contexto=cContexto(
          inst.substr(fimPadrao+1, fimContexto-(fimPadrao+1)),
          pos=0
        );
      
      if(!significadoOmitido)
      {
        if(contextoOmitido)
          buildSignificado(inst.substr(fimPadrao+2,inst.size()-(fimPadrao+2)));
        else
          buildSignificado(
            inst.substr(fimContexto+2,inst.size()-(fimContexto+2))
          );
      }
    }
    // TODO: adicionar o build não validado:
    else throw "O validated deveria ter sido lançado antes do build!";
  }
  
  pMatch::charClass cInst::mRotulo      = pMatch::charClass("[:,=-]");
  pMatch::charClass cInst::mPadrao      = pMatch::charClass("[=-]");
  // TODO: Adicionar o operador @ para o contexto:
  // TODO: Adicionar operadores < > = para o contexto:
  pMatch::charClass cInst::mSignificado = pMatch::charClass(";");
  pMatch::charClass cInst::notBlank     = pMatch::charClass("[^ \t\n]");
  pMatch::charClass
  cInst::mContexto = pMatch::charClass("[^][a-zA-Z0-9._ \t\n]");

  cInst::cInst() {}

  cInst::cInst(const char* inst, int ID) : cInst(string(inst),ID) {}
  cInst::cInst(string inst, int ID)
  {
    this->ID = ID;
    
    validate(inst);
    build(inst);
  }
  
/* * * * * END cInst * * * * */

/* * * * * START TEST cInst * * * * */

/*
 *  Teste cInst::cInst()
 */
// Descomente para testar:
 /*
int main(int argc, char* argv[])
{
  using namespace pMatch;
  
  cInst inst;
  int ID;
  
  cout << " * * * * * TESTE cInst(string) * * * * *\n\n";
  
  cout << "  @Testes com a validação:" << endl << endl;
  
  try{
  cout << 1 << endl;
  cout << "Teste inicializado" << endl;
  cInst("teste: teste - teste => teste", ID=0);
  cout << "      " << "inicializado" << endl << endl;
  }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

  try{
  cout << 1.1 << endl;
  cout << "Teste erro lançado!" << endl;
  cInst(":teste: teste - teste => teste", ID=0);
  cout << "      " << "inicializado" << endl << endl;
  }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

  try{
  cout << 1.2 << endl;
  cout << "Teste erro lançado!" << endl;
  cInst(",teste: teste - teste => teste", ID=0);
  cout << "      " << "inicializado" << endl << endl;
  }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

  try{
  cout << 1.3 << endl;
  cout << "Teste erro lançado!" << endl;
  cInst("-teste: teste - teste => teste", ID=0);
  cout << "      " << "inicializado" << endl << endl;
  }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

  try{
  cout << 2 << endl;
  cout << "Teste inicializado" << endl;
  cInst("teste: teste - teste => teste", ID=0);
  cout << "      " << "inicializado" << endl << endl;
  }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

  try{
  cout << 2.1 << endl;
  cout << "Teste inicializado" << endl;
  cInst("teste\\: teste \\- teste => teste", ID=0);
  cout << "      " << "inicializado" << endl << endl;
  }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

  try{
  cout << 3 << endl;
  cout << "Teste inicializado" << endl;
  cInst("teste,teste,teste: teste - teste => teste", ID=0);
  cout << "      " << "inicializado" << endl << endl;
  }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

  try{
  cout << 3.1 << endl;
  cout << "Teste erro lançado!" << endl;
  cInst("teste,teste,: teste - teste => teste", ID=0);
  cout << "      " << "inicializado" << endl << endl;
  }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

  try{
  cout << 3.1 << endl;
  cout << "Teste erro lançado!" << endl;
  cInst("teste,teste,  : teste - teste => teste", ID=0);
  cout << "      " << "inicializado" << endl << endl;
  }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

  try{
  cout << 3.2 << endl;
  cout << "Teste erro lançado!" << endl;
  cInst("teste,,teste: teste - teste => teste", ID=0);
  cout << "      " << "inicializado" << endl << endl;
  }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

  try{
  cout << 3.2 << endl;
  cout << "Teste erro lançado!" << endl;
  cInst("teste,  ,teste: teste - teste => teste", ID=0);
  cout << "      " << "inicializado" << endl << endl;
  }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

  try{
  cout << 3.3 << endl;
  cout << "Teste erro lançado!" << endl;
  cInst(" ,teste,teste: teste - teste => teste", ID=0);
  cout << "      " << "inicializado" << endl << endl;
  }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

  try{
  cout << 3.3 << endl;
  cout << "Teste erro lançado!" << endl;
  cInst("   ,teste,teste: teste - teste => teste", ID=0);
  cout << "      " << "inicializado" << endl << endl;
  }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

  try{
  cout << 4 << endl;
  cout << "Teste inicializado" << endl;
  cInst("teste,teste: teste - teste => teste", ID=0);
  cout << "      " << "inicializado" << endl << endl;
  }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

  try{
  cout << 4.1 << endl;
  cout << "Teste inicializado" << endl;
  cInst("teste,teste:teste - teste => teste", ID=0);
  cout << "      " << "inicializado" << endl << endl;
  }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

  try{
  cout << 5 << endl;
  cout << "Teste inicializado" << endl;
  cInst("rotulo: padrao", ID=0);
  cout << "      " << "inicializado" << endl << endl;
  }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

  try{
  cout << 5 << endl;
  cout << "Teste inicializado" << endl;
  cInst("padrao => significado", ID=0);
  cout << "      " << "inicializado" << endl << endl;
  }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

  try{
  cout << 5 << endl;
  cout << "Teste inicializado" << endl;
  cInst("padrao - contexto => significado", ID=0);
  cout << "      " << "inicializado" << endl << endl;
  }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

  // Rotulo omitido e um \0 após o padrão, isso gera erro:
  try{
  cout << 5.1 << endl;
  cout << "Teste erro lançado!" << endl;
  cInst("padrao", ID=0);
  cout << "      " << "inicializado" << endl << endl;
  }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

  try{
  cout << 6 << endl;
  cout << "Teste inicializado" << endl;
  cInst("rotulo: padrao - contexto => significado", ID=0);
  cout << "      " << "inicializado" << endl << endl;
  }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

  try{
  cout << 6.1 << endl;
  cout << "Teste inicializado" << endl;
  cInst("rotulo: padrao- contexto => significado", ID=0);
  cout << "      " << "inicializado" << endl << endl;
  }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

  try{
  cout << 6.2 << endl;
  cout << "Teste inicializado" << endl;
  cInst("rotulo: padrao -contexto => significado", ID=0);
  cout << "      " << "inicializado" << endl << endl;
  }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

  try{
  cout << 7 << endl;
  cout << "Teste inicializado" << endl;
  cInst("rotulo: padrao - variavel.de_contexto, v1 => significado", ID=0);
  cout << "      " << "inicializado" << endl << endl;
  }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

//xxx
  try{
  cout << 7.1 << endl;
  cout << "Teste inicializado" << endl;
  cInst("rotulo: padrao - variavel.de_contexto, v1=2 => significado", ID=0);
  cout << "      " << "inicializado" << endl << endl;
  }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }
  
  try{
  cout << 7.2 << endl;
  cout << "Teste inicializado" << endl;
  cInst("rotulo: padrao - variavel.de_contexto; v1 => significado", ID=0);
  cout << "      " << "inicializado" << endl << endl;
  }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }
  
cout
  << " O teste abaixo realça um problema: " << endl
  << " 'rot: pad - v1, v2' é correto, mas 'rot: pad - v1, v2,' não é."<< endl
  << " 'rot: pad - v1; v2; é correto, e 'rot: pad - v1; v2' também é."<< endl
  << " Isso deverá ser corrigido no futuro." << endl;
// TODO: Corrigir o problema citado acima.
  try{
  cout << 7.3 << endl;
  cout << "Teste inicializado" << endl;
  cInst("rotulo: padrao - variavel.de_contexto; v1; => significado", ID=0);
  cout << "      " << "inicializado" << endl << endl;
  }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }
  
  try{
  cout << 8 << endl;
  cout << "Teste inicializado" << endl;
  cInst("rotulo: padrao - contexto, v1, v2 => significado", ID=0);
  cout << "      " << "inicializado" << endl << endl;
  }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }
  
  try{
  cout << 8.1 << endl;
  cout << "Teste erro lançado!" << endl;
  cInst("rotulo: padrao - , v1, v2 => significado", ID=0);
  cout << "      " << "inicializado" << endl << endl;
  }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

  try{
  cout << 8.2 << endl;
  cout << "Teste erro lançado!" << endl;
  cInst("rotulo: padrao - contexto, , v2 => significado", ID=0);
  cout << "      " << "inicializado" << endl << endl;
  }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

  try{
  cout << 8.3 << endl;
  cout << "Teste erro lançado!" << endl;
  cInst("rotulo: padrao - contexto, v1, => significado", ID=0);
  cout << "      " << "inicializado" << endl << endl;
  }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

  try{
  cout << 9 << endl;
  cout << "Teste inicializado" << endl;
  cInst("rotulo: padrao - contexto => sig1; sig2; sig3", ID=0);
  cout << "      " << "inicializado" << endl << endl;
  }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }
  
  try{
  cout << 9 << endl;
  cout << "Teste inicializado" << endl;
  cInst("rotulo: padrao - contexto => sig1; sig2; sig3;", ID=0);
  cout << "      " << "inicializado" << endl << endl;
  }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }
  
  try{
  cout << 9.1 << endl;
  cout << "Teste erro lançado!" << endl;
  cInst("rotulo: padrao - contexto => ; sig2; sig3", ID=0);
  cout << "      " << "inicializado" << endl << endl;
  }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }

  try{
  cout << 9.2 << endl;
  cout << "Teste erro lançado!" << endl;
  cInst("rotulo: padrao - contexto => sig1; ; sig3", ID=0);
  cout << "      " << "inicializado" << endl << endl;
  }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }
  
  try{
  cout << 9.3 << endl;
  cout << "Teste erro lançado!" << endl;
  cInst("rotulo: padrao - contexto => sig1; sig2; ;", ID=0);
  cout << "      " << "inicializado" << endl << endl;
  }catch(const char* c){ cout << "      " << "erro lançado!" << endl << endl; }
  
  cout << "TESTE DE CONSTRUCAO e IMPRESSAO:" << endl << endl;
  
  try{
  cout << 1 << endl;
  cout << "Teste rotulo: \"padrao\" - contexto => \"significado\";" << endl;
  inst = cInst("rotulo: padrao - contexto => significado;", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 2 << endl;
  cout << "Teste rotulo: \"padrao\" - contexto => \"significado\";" << endl;
  inst = cInst("       rotulo: padrao - contexto => significado;", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 3 << endl;
  cout << "Teste rotulo: \"padrao\" - contexto => \"significado\";" << endl;
  inst = cInst("rotulo          : padrao - contexto => significado;", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 4 << endl;
  cout << "Teste rotulo: \"padrao\" - contexto => \"significado\";" << endl;
  inst = cInst("rotulo:           padrao - contexto => significado;", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 5 << endl;
  cout << "Teste rotulo: \"padrao\" - contexto => \"significado\";" << endl;
  inst = cInst("rotulo: padrao          - contexto => significado;", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 6 << endl;
  cout << "Teste rotulo: \"padrao\" - contexto => \"significado\";" << endl;
  inst = cInst("rotulo: padrao -            contexto => significado;", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 7 << endl;
  cout << "Teste rotulo: \"padrao\" - contexto => \"significado\";" << endl;
  inst = cInst("rotulo: padrao - contexto          => significado;", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 8 << endl;
  cout << "Teste rotulo: \"padrao\" - contexto => \"significado\";" << endl;
  inst = cInst("rotulo: padrao - contexto =>          significado;", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 9 << endl;
  cout << "Teste rotulo: \"padrao\" - contexto => \"significado\";" << endl;
  inst = cInst("rotulo: padrao - contexto => significado         ;", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 10 << endl;
  cout << "Teste rotulo: \"padrao\" - contexto => \"significado\";" << endl;
  inst = cInst("rotulo: padrao - contexto => significado;         ", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 11 << endl;
  cout << "Teste rotulo: \"padrao\" - contexto => \"significado\";" << endl;
  inst = cInst("rotulo: padrao - contexto => \"significado\";", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 12 << endl;
  cout << "Teste rotulo: \"padrao\" - contexto => \"significado\";" << endl;
  inst = cInst("rotulo: \"padrao\" - contexto => \"significado\";", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 13 << endl;
  cout << "Teste rotulo: \" padrao\" - contexto => \"significado\";" << endl;
  inst = cInst("rotulo: \" padrao\" - contexto => \"significado\";", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 14 << endl;
  cout << "Teste rotulo: \"padrao \" - contexto => \"significado\";" << endl;
  inst = cInst("rotulo: \"padrao \" - contexto => \"significado\";", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 15 << endl;
  cout << "Teste rotulo: \"padrao\" - contexto => \" significado\";" << endl;
  inst = cInst("rotulo: \"padrao\" - contexto => \" significado\";", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 16 << endl;
  cout << "Teste rotulo: \"padrao\" - contexto => \"significado \";" << endl;
  inst = cInst("rotulo: \"padrao\" - contexto => \"significado \";", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 17 << endl;
  cout << "Teste r1, r2, r3: \"padrao\" - contexto => \"significado\";"<< endl;
  inst = cInst("r1,r2,r3:\"padrao\"-contexto=>\"significado\";", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 17.1 << endl;
  cout << "Teste r1, r2, r3: \"padrao\" - contexto => \"significado\";"<< endl;
  inst = cInst("r1 ,r2,r3: \"padrao\" - contexto => \"significado\";", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 17.2 << endl;
  cout << "Teste r1, r2, r3: \"padrao\" - contexto => \"significado\";"<< endl;
  inst = cInst("r1, r2,r3: \"padrao\" - contexto => \"significado\";", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 17.3 << endl;
  cout << "Teste r1, r2, r3: \"padrao\" - contexto => \"significado\";"<< endl;
  inst = cInst(" r1,r2,r3: \"padrao\" - contexto => \"significado\";", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 17.4 << endl;
  cout << "Teste r1, r2, r3: \"padrao\" - contexto => \"significado\";"<< endl;
  inst = cInst("r1,r2,r3 : \"padrao\" - contexto => \"significado\";", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 18 << endl;
  cout << "Teste rotulo: \"padrao\" - c1, c2, c3 => \"significado\";" << endl;
  inst = cInst("rotulo: \"padrao\" - c1,c2,c3 => \"significado\";", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 18.1 << endl;
  cout << "Teste rotulo: \"padrao\" - c1, c2, c3 => \"significado\";" << endl;
  inst = cInst("rotulo: \"padrao\" - c1     ,c2,c3 => \"significado\";", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 18.2 << endl;
  cout << "Teste rotulo: \"padrao\" - c1, c2, c3 => \"significado\";" << endl;
  inst = cInst("rotulo: \"padrao\" - c1,      c2,c3 => \"significado\";", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 18.3 << endl;
  cout << "Teste rotulo: \"padrao\" - c1, c2, c3 => \"significado\";" << endl;
  inst = cInst("rotulo: \"padrao\" - c1,c2    ,c3 => \"significado\";", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 18.4 << endl;
  cout << "Teste rotulo: \"padrao\" - c1, c2, c3 => \"significado\";" << endl;
  inst = cInst("rotulo: \"padrao\" - c1,c2,     c3 => \"significado\";", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 18.5 << endl;
  cout <<"Teste rotulo: \"padrao\" - _con.texto[0] => \"significado\";" << endl;
  inst = cInst("rotulo: \"padrao\" - _con.texto[0] => \"significado\";", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 19 << endl;
  cout << "Teste rotulo: \"padrao\" - contexto => #!driver: \"sig\";" << endl;
  inst = cInst("rotulo: \"padrao\" - contexto => #!driver: sig;", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 19.1 << endl;
  cout << "Teste rotulo: \"padrao\" - contexto => #!driver: \"sig\";" << endl;
  inst = cInst("rotulo: \"padrao\" - contexto => #!  driver: sig;", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 19.2 << endl;
  cout << "Teste rotulo: \"padrao\" - contexto => #!driver: \"sig\";" << endl;
  inst = cInst("rotulo: \"padrao\" - contexto => #!driver  : sig;", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 19.3 << endl;
  cout << "Teste rotulo: \"padrao\" - contexto => #!driver: \"sig\";" << endl;
  inst = cInst("rotulo: \"padrao\" - contexto => #!driver:sig;", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 19.4 << endl;
  cout << "Teste rotulo: \"padrao\" - contexto => #!driver: \"sig\";" << endl;
  inst = cInst("rotulo: \"padrao\" - contexto => #!driver:  sig  ;", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 20 << endl;
  cout << "Teste rotulo: \"padrao\" - contexto => \"#!driver: sig;\";" << endl;
  inst = cInst("rotulo: \"padrao\" - contexto => \"#!driver: sig;\"", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 21 << endl;
  cout << "Teste rotulo: \"padrao\" - contexto => #!driver: \"sig\";" << endl;
  inst = cInst("rotulo: \"padrao\" - contexto => #!driver: \"sig\"", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 22 << endl;
  cout << "Teste rotulo: \"padrao\" - contexto => #!driver: \"sig;\";" << endl;
  inst = cInst("rotulo: \"padrao\" - contexto => #!driver: \"sig;\"", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 23 << endl;
  cout << "Teste rotulo: \"padrao\" - contexto => \"sig\";" << endl;
  inst = cInst("rotulo: \"padrao\" - contexto => \"sig\"", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 24 << endl;
  cout << "Teste rotulo: \"padrao\" - contexto => \"sig@nif;cado\";" << endl;
  inst = cInst("rotulo: \"padrao\" - contexto => \"sig@nif;cado\"", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 24.1 << endl;
  cout << "Teste rotulo: \"padrao\" - contexto => \"@nif;cado\";" << endl;
  inst = cInst("rotulo: \"padrao\" - contexto => \"@nif;cado\"", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 24.2 << endl;
  cout << "Teste rotulo: \"padrao\" - contexto => \"sig@nif;\";" << endl;
  inst = cInst("rotulo: \"padrao\" - contexto => \"sig@nif;\"", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 24.3 << endl;
  cout << "Teste rotulo: \"padrao\" - contexto => \"sig@nif;cado\";" << endl;
  inst = cInst("rotulo: \"padrao\" - contexto => sig@nif;cado", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 24.4 << endl;
  cout << "Teste rotulo: \"padrao\" - contexto => \"sig@nif;cado\";" << endl;
  inst = cInst("rotulo: \"padrao\" - contexto => \"sig@nif  ;cado\"", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 24.5 << endl;
  cout << "Teste rotulo: \"padrao\" - contexto => \"sig@nif;cado\";" << endl;
  inst = cInst("rotulo: \"padrao\" - contexto => \"sig@  nif;cado\"", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 24.6 << endl;
  cout << "Teste rotulo: \"padrao\" - contexto => \"sig@n.i.f;cado\";" << endl;
  inst = cInst("rotulo: \"padrao\" - contexto => \"sig@n.i.f;cado\"", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 24.7 << endl;
  cout << "Teste rotulo: \"padrao\" - contexto => \"sig@n.i.f[0];cado\";"<<endl;
  inst = cInst("rotulo: \"padrao\" - contexto => \"sig@n.i.f[0];cado\"", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 24.8 << endl;
  cout << "Teste rotulo: \"padrao\" - contexto => \"sig@n.i[0].f;cado\";"<<endl;
  inst = cInst("rotulo: \"padrao\" - contexto => \"sig@n.i[0].f;cado\"", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 25 << endl;
  cout << "Teste rotulo: \"padrao\" - contexto => \"sig1\"; \"sig2\"; \"sig3\";"<<endl;
  inst = cInst("rotulo: \"padrao\" - contexto => sig1; sig2; sig3", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  try{
  cout << 25 << endl;
  cout << "Teste rotulo: \"padrao\" - contexto => \"@sig1;\"; \"@sig2;\"; \"@sig3;\";"<<endl;
  inst = cInst("rotulo: \"padrao\" - contexto => @sig1;; @sig2;; @sig3;", ID=0);
  cout << "      " << inst.str() << endl << endl;
  }catch(const char* c){ cout << "      " << "error: " << c <<  endl << endl; }
  
  return 0;
}
// */

/* * * * * END TEST cInst * * * * */

// Valida uma referencia à variável no formato: "a.b.c[5][10].d"
// Com a possibilidade de aceitar espaços entre os termos com letras e numeros.
// TODO: mover esse validate addr para dentro da classe cExpressao
void validate_addr(std::string contexto, int& pos)
{
  // O código abaixo pode ser resumido na seguintes duas linhas:
  /*
  if(arrayClass(" *[A-Za-z_][a-zA-Z0-9_]* *(\". *[a-zA-Z_][a-zA-Z0-9_]*\",\"\\[[0-9][0-9]*]\")* *(\"[=><]\",\"\")").match(contexto)) return;
  else throw "Error!";
  */
  // Obviamente não vou usar isso porque fica bem menos claro, e menos eficiente.
  
  bool bArray=false;
  auto mVar = charClass("[a-zA-Z0-9_]");
  
  // Ignore quaisquer caracteres em branco:
  notBlank.find(contexto,pos);
  
  // Se o primeiro caractere não for válido:
  if(!charClass("[a-zA-Z_]").match(contexto[pos]))
    throw "Nome de variável iniciado com caractere inválido!, use apenas [a-zA-Z_]: validate_addr()";

//cout << "contexto: \"" << contexto << '"' << endl;
//cout << "pos: " << pos << endl;
//cout << "bArray: " << bArray << endl;
  
  // Inicie a validação do nome, cada loop corresponde a um item
  // na arvore de variáveis do tipo: "a.b.c[5][10].d"
  while(contexto[pos])
  {
    string var;
//cout << "loop!" << endl;
//cout << "Char: " << contexto[pos] << endl;
    
    // Verifico se a variável é do tipo de acesso à array: '[0]' ou '[100]'
    if(!bArray)
    {
      // Elimine espaços em branco antes do nome da variável.
      while(blank.match(contexto[pos])) pos++;
      
      // Leia todas as letras do nome da variável.
      while(mVar.match(contexto[pos])) pos++;
      
      // Tratando o fim do nome de um item do endereço de variável
      // do tipo da variável c ou d no exemplo a seguir: "a.b.c.d[10]":
      if(charClass("[[.;, \t\n=<>]").match(contexto[pos]) || !contexto[pos])
      {
        // Encontre a próxima posição que não seja branca.
        if(contexto[pos]==' ') notBlank.find(contexto,pos);
        
//cout << "contexto[pos]: " << contexto[pos] << endl;
        switch(contexto[pos])
        {
          case '[':
            bArray=true;
            pos++;
          break;
          case '.': pos++; break;
          case '=': return;
          case '<': return;
          case '>': return;
          case ';': return;
          case ',': return;
          case '\0': return;
          default:
          
          // Se após o espaço encontrou-se outro inicio de nome de variável:
          if(charClass("[a-zA-Z_0-9]").match(contexto[pos]))
            throw "O nome da variável contem espaços ou falta um operador!:validate_addr";
        }
      }
    }
    else
    // Nesse caso estamos lendo algo do tipo "[10]"
    {
      // Despreze os caracteres em branco:
      notBlank.find(contexto,pos);
      
      // Leia o número até o final:
      charClass("[^0-9]").find(contexto,pos);
      
      // Despreze os demais caracteres em branco:
      notBlank.find(contexto,pos);
      
      if(contexto[pos]!=']')
        throw "Esperado ']' no endereço de variável!: cContexto::validate()";
      else pos++;
      
      // Despreze quaisquer caracteres em branco:
      notBlank.find(contexto,pos);
      
      switch(contexto[pos])
      {
//xxx
        // Aponte o iterador 'pos' para a posição de início da próxima variável:
        case '.': bArray = false; break;
        case '[': bArray = true; break;
        
        // Se chegou ao fim do endereço de variável retorne:
        case '>':  return;
        case '<':  return;
        case '=':  return;
        case ';':  return;
        case ',':  return;
        case '\0': return;
        
        default:
          if(charClass("[a-zA-Z_0-9]").match(contexto[pos]))
            throw "Esperado '.', mas encontrado: '[a-zA-Z_0-9]' em endereço de variável!: cContexto::validate()";
      } 
      
      // Se chegou até aqui é porque encontrou '.' ou '[', logo:
      pos++;
    }
  }
}

void cContexto::readOperand(std::string contexto, int& pos)
{
  // Despreze os caracteres brancos:
  notBlank.find(contexto, pos);
  
  // Se tiver encontrado o inicio de um endereço de variável:
  if(charClass("[a-zA-Z_]").match(contexto[pos]))
{
//cout << "go teste!" << endl;
    // Verifica até onde a variável vai:
    validate_addr(contexto,pos);
//cout << "end teste!" << endl;
}
  // Se tiver encontrado o inicio de um caractere numerico:
  else if(charClass("[0-9]").match(contexto[pos]))
    charClass("[^0-9]").find(contexto,pos);
  // Se tiver encontrado o início de uma string:
  else if(contexto[pos]=='"')
  {
    pos++;
    while(contexto[pos-1]=='\\' || contexto[pos]!='"') pos++;
  }
  
//cout << "contexto: \"" << contexto << '"' << endl;
//cout << "contexto[pos]: " << contexto[pos] << endl;
//cout << "pos: " << pos << endl;
  // Despreze os caracteres brancos:
  if(contexto[pos]=='"') pos++;
  notBlank.find(contexto,pos);

//cout << "Contexto: \"" << contexto << '"' << endl;
//cout << "Pos: " << pos << endl;
//cout << "Char: " << (int)(contexto[pos]) << endl;
//cout << "Bool: " << (contexto[pos]!='\0') << endl;
  
  // Caso o caractere após o operando não seja: =<>;, ou \0:
  if(!charClass("[-=<>+&|!;,]").match(contexto[pos]) && contexto[pos]!='\0')
    throw "Caractere inválido no contexto!: cContexto::validate()";
}

// Verifica se a string recebida pela construtora
// é uma string válida para ser lida pelo build().
// Caso contrário lança o erro correspondente.
void cContexto::validate(std::string contexto)
{
  int pos=0;
  
  // Para cada item do contexto: " a = b; b < 3; c > 4 "
  while(contexto[pos])
  {
    // Leia o primeiro operando:
    readOperand(contexto,pos);
    
    // Um loop para cada par operador, operando:
    while(contexto[pos]!='\0' && contexto[pos]!=';')
    {
      // Pule o operador:
      pos++;
      // Leia o próximo operando:
      readOperand(contexto,pos);
    }
    
    if(contexto[pos]==';' || contexto[pos]==',')
    {
      pos++; continue;
    }
    else return;
  }
}

expItem cExpressao::getOperando(std::string exp, int& pos)
{
  int inicio;
  std::pair<eTipo,std::string> resp;

  // Despreze espaços em branco:
  notBlank.find(exp, pos);
  
  // cout << "exp_operando: '" << exp << "'" << endl;
  // cout << "pos: " << pos << endl;
  inicio = pos;
  
  if(exp[pos]=='"')
  {
    pos++;
    while(exp[pos-1]=='\\' || exp[pos]!='"') pos++;
    resp = {STR,exp.substr(inicio+1,pos-inicio-1)};
  }
  else if(exp.substr(pos,4)=="true")
  {
    resp = {BOOL, string("true")};
    pos+=4;
  }
  else if(exp.substr(pos,5)=="false")
  {
    resp = {BOOL, string("")};
    pos+=5;
  }
  else if(charClass("[0-9.-]").match(exp[pos]))
  // Caso tenha encontrado um número
  {
    charClass("[^0-9.e-]").find(exp,pos);
    resp = {NUM,exp.substr(inicio,pos-inicio)};
  }
  else
  // Caso tenha encontrado um endereço de variável:
  {
    for( ; (unsigned)pos<exp.length(); pos++)
      if(opClass.match(exp[pos])) break;
    // validate_addr(exp,pos);
    // Encontre o fim do nome da variável:
    int aux = pos-1;
    while(blank.match(exp[aux])) aux--;
    resp = {VAR,exp.substr(inicio,aux+1-inicio)};
  }
  
  // Despreze os caracteres brancos:
  if(exp[pos]=='"') pos++;
  notBlank.find(exp,pos);
  
  // cout << "Var? " << (resp.first==VAR?"true":"false") << endl;
  return resp;
}

expItem cExpressao::getOperador(std::string exp, int& pos)
{
  int inicio = pos;

  // Encontra o fim do nome do operador.
  while(opClass.match(exp[pos])) pos++;

  string op = exp.substr(inicio,pos-inicio);

  if(!opMap.count(op))
    throw "Operador não existente! ou incorreto";

  notBlank.find(exp,pos);

  return {OP,op};
}

void cExpressao::build(std::string exp)
{
  // O mapa abaixo associa cada operador com uma prioridade.
  // Essa prioridade é maior para multiplicações
  // do que para somas por exemplo.
  map<string, int> prioridade;

  /*
    Segue abaixo os operadores reconhecidos e
    suas respectivas prioridades:
    3 "!"
      
    2 "*"
      "&&"
      "/"
      
    1 "+"
      "-"
      "||"
      
    0 "=="
      "!="
      "<"
      ">"
      "<="
      ">="
  */

  int priority=0;
  int new_priority=0;

  list<expItem> operadores;
  list<expItem> operandos;
  list<expItem> opUnitarios;
  expItem item;
  
  // loop:
  for(int i=0; (unsigned)i<exp.length();)
  {
    // Find What:
    if(opClass.match(exp[i]))
    {
      // IF operador call getOperador - return operator priority.
      item = getOperador(exp,i);
      if(item.second==string("!"))
        opUnitarios.push_back(item);
      else {
        new_priority = opMap[item.second];
        operadores.push_back(item);
      }
    }
    else
    {
      // IF exp[i] == operando: call getOperando.
      item = getOperando(exp,i);
      operandos.push_back(item);
      
      // Se após adicionar um item, houver algo na pilha
      // de operadores unitários, desempilhe-os:
      if(opUnitarios.size()>0) {
        opUnitarios.reverse();
        this->splice(this->end(),operandos);
        this->splice(this->end(),opUnitarios);
      }
    }

    // IF priority > new_priority push back both lists and empty them.
    if(priority > new_priority || (unsigned)i==exp.length())
    {
      if(priority > new_priority) {
        // Salvo e removo o último elemento
        item = operadores.back();
        operadores.pop_back();
      }

      // Inverto os operadores,
      operadores.reverse();
      // E desempilho as duas pilhas na expressão:
      this->splice(this->end(),operandos);
      this->splice(this->end(),operadores);

      if(priority > new_priority) {
        // Agora re-construo a nova pilha de operadores:
        priority=new_priority;
        operadores.push_back(item);
      }
      else priority = 0;
    }
    else if(priority <= new_priority) priority = new_priority;
    
  // end_loop
  }
}

cExpressao::cExpressao(std::string exp, int& pos)
{
  build(exp.substr(pos,exp.length()-pos));
  pos = exp.length();
}

// TODO: Mover essa função para um lugar adequado.
// Essa função recebe um nome de variável, e o conjunto de
// variáveis globais e locais, e recupera a variável
// desejada entre elas, obedecendo a prioridade das variaveis locais.
vars::cVar& getVar(
  std::string var_name, vars::cVar& local, vars::cVar& global
)
{
  auto value = local.getVar(var_name);
  if(value) return *value;
  else if((value=global.getVar(var_name)))
    return *(global.getVar(var_name));
  else return global.addVar(var_name,"");
}

// Recebe um conjunto de variáveis locais e globais, e calcula
// o valor da expressão cExpressao com estas variáveis.
std::string cExpressao::eval(vars::cVar local, vars::cVar global)
{
  string resp;
  list<string> pilha;

  for(auto& item : (*this))
  {
    if(item.first==STR || item.first==NUM || item.first==BOOL)
      pilha.push_back(item.second);
    if(item.first==VAR)
      pilha.push_back(getVar(item.second, local, global).getValor());
    if(item.first==OP)
    {
      bool single=false;
      string a,b;
      if(pilha.size()==1)
      {
        single = true;
        b = pilha.back(); pilha.pop_back();
      }
      else
      {
        single = false;
        b = pilha.back(); pilha.pop_back();
        a = pilha.back(); pilha.pop_back();
      }

      char op = item.second[0];
      // Faça um switch com o operador que deve estar na primeira posição:
      switch(op)
      {
        case '=': resp = op_equal(a,b); break;
        case '<': resp = op_menor(a,b); break;
        case '>': resp = op_maior(a,b); break;
        case '&': resp = op_and(a,b);   break;
        case '|': resp = op_or(a,b);    break;
        case '+': resp = op_sum(a,b);   break;
        case '-': resp = op_sub(a,b);   break;
        case '*': resp = op_mul(a,b);   break;
        case '/': resp = op_div(a,b);   break;
        case '!':
          if(item.second[1]=='=')
            resp = op_diff(a,b);
          else
          {
            if(!single)
              pilha.push_back(a);
            resp = op_not(b);
          }
        break;
        
        default:
          throw "Operador não reconhecido! cExpressao::eval()";
      }

      pilha.push_back(resp);
    }
  }

  if(pilha.size()!=1)
    throw "Expressão não retornou um único elemento!:cExpressao::eval()";


  // cout << "global: " << global.str() << endl;
  return pilha.front();
}

std::string cExpressao::op_equal(std::string a, std::string b)
{
  if(numeric(a) && numeric(b))
    return atoi(a.c_str())==atoi(b.c_str())?"true":"";
  
  if(a==b) return string("true");
  else return string("");
}

std::string cExpressao::op_diff(std::string a, std::string b)
{
  if(numeric(a) && numeric(b))
    return atoi(a.c_str())!=atoi(b.c_str())?"true":"";

  if(a!=b) return string("true");
  else return string("");
}

std::string cExpressao::op_menor(std::string a, std::string b)
{
  if(numeric(a) && numeric(b))
    return atoi(a.c_str())<atoi(b.c_str())?"true":"";

  if(a<b) return string("true");
  else return string("");
}

std::string cExpressao::op_maior(std::string a, std::string b)
{
  if(numeric(a) && numeric(b))
    return atoi(a.c_str())>atoi(b.c_str())?"true":"";

  if(a>b) return string("true");
  else return string("");
}

std::string cExpressao::op_and(std::string a, std::string b)
{
  string empty;
  if(a!=empty && b!=empty) return string("true");
  else return string("");
}

std::string cExpressao::op_or(std::string a, std::string b)
{
  string empty;
  if(a!=empty || b!=empty) return string("true");
  else return string("");
}

bool cExpressao::numeric(std::string str)
{
  int pos;
  // Se encontrar qualquer caractere não numerico:
  if(charClass("[^-eE0-9.]").find(str,pos=0)) return false;

  int dot=0;
  for(auto c : str) if(c=='.') dot++;
  if(dot>1) return false;
  
  return true;
}

std::string cExpressao::float_to_str (double number){
  std::ostringstream buff;
  buff << number;
  return buff.str();   
}

std::string cExpressao::op_sum(std::string a, std::string b)
{
  if(!numeric(a)) return string("");
  if(!numeric(b)) return string("");
  
  double af=atof(a.c_str());
  double bf=atof(b.c_str());

  return float_to_str(af+bf);
}

std::string cExpressao::op_sub(std::string a, std::string b)
{
  if(!numeric(a)) return string("");
  if(!numeric(b)) return string("");
  
  double af=atof(a.c_str());
  double bf=atof(b.c_str());

  return float_to_str(af-bf);
}

std::string cExpressao::op_mul(std::string a, std::string b)
{
  if(!numeric(a)) return string("");
  if(!numeric(b)) return string("");
  
  double af=atof(a.c_str());
  double bf=atof(b.c_str());

  return float_to_str(af*bf);
}

std::string cExpressao::op_div(std::string a, std::string b)
{
  if(!numeric(a)) return string("");
  if(!numeric(b)) return string("");
  
  double af=atof(a.c_str());
  double bf=atof(b.c_str());

  return float_to_str(af/bf);
}

std::string cExpressao::op_not(std::string str)
{
  if(str==string("")) return string("true");
  else return string("");
}

void cContexto::build(std::string contexto)
{
  int pos=0;
  
  // Para cada item do contexto: " a = b; b < 3; c > 4 "
  while(contexto[pos])
  {
    expList.push_back(cExpressao(contexto, pos));
    
    if(contexto[pos]==';')
    {
      pos++; continue;
    }
    else return;
  }
}

// Avalia cada uma das expressões do contexto.
// Caso uma delas seja falsa o contexto é considerado falso.
bool cContexto::eval(vars::cVar& local, vars::cVar& global)
{
  for(auto& exp : this->expList)
    if(exp.eval(local,global)==string("")) return false;
  return true;
}

cContexto::cContexto(std::string contexto)
{
  validate(contexto);
  build(contexto);
}

cContexto::cContexto(std::string contexto, int& pos)
{
  int inicio = pos;
  // Encontre o fim do contexto:
  while((contexto[pos]!='=' || contexto[pos+1]!='>') && contexto[pos]) pos++;
  
  contexto = contexto.substr(inicio, pos-inicio);
  
  validate(contexto);
  build(contexto);
}

void executaSig(vars::cVar local, cSignificado sig)
{
  int aux=0;
  string str = sig.texto;
  
  for(auto var = sig.variaveis.rbegin(); var!=sig.variaveis.rend(); var++)
  {
    // Pegue o valor da variável:
    string str_var = getVar(var->first, local, ambiente::global).getValor();
    
    str.insert(var->second+aux, str_var);
    aux+=str_var.length();
  }
  
  if(sig.driver.length()>0)
    ambiente::drivers[sig.driver]->run(str);
  
  else
    banco::execInst(str);
}

void executa(cInst inst, pMatch::tInterpretacao escopo_local)
{
  // Variáveis locais:
  local = vars::cVar("local",escopo_local);

  list<cSignificado> sigs = inst.getSignificados();
  
  for(auto& sig : sigs)
    executaSig(local, sig);
}


