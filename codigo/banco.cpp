#include "banco.hpp"
#include "ambiente.hpp"
#include <stdlib.h>
#include <sstream>
#include <cstring>

using namespace std;
using namespace pMatch;

// charClasses utilizados para lidar com caracteres em branco:
charClass isBlank = charClass("[ \t\n]");

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

cInst* banco::addInst(string inst, int level) {
  int ID = getUniqueID();
  instrucoes[level][ID] = cInst(inst, ID);
  return &(instrucoes[level][ID]);
}

cInst* banco::addInst(string inst) {
  int ID = getUniqueID();
  instrucoes[def_level][ID] = cInst(inst, ID);
  return &(instrucoes[def_level][ID]);
}

pair<cInst*, int> banco::getInst(int ID) {
  int i;
  bool found=false;
  for(i=def_level; i>=0; i--)
    if(instrucoes[i].count(ID)) {
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
string banco::str() {
  std::string str;
  for(int i=0; (unsigned)i<instrucoes.size(); i++) {
    for(auto j=instrucoes[i].begin(); j!=instrucoes[i].end(); j++) {
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
list<string> banco::strList() {
  std::list<std::string> list;
  for(int i=0; (unsigned)i<instrucoes.size(); i++) {
    for(auto j=instrucoes[i].begin(); j!=instrucoes[i].end(); j++) {
      std::ostringstream oss;
      oss <<"inst["<< i <<"]["<< j->first <<"]: «"<<j->second.str() << "»";
      list.push_back(oss.str());
    }
  }
  return list;
}

// Retorna a lista de rotulos em formato de string de lista.
list<string> banco::rotList() {
  list<string> resp;
  
  for(auto& rot : rotulos) {
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
  for(auto j=var.lInt.begin(); j!= var.lInt.end(); ) {
    auto v = vars::cObject(*j);
    // Se o contexto não for válido:
    //if(contexto.eval(vars::cObject(j->var),ambiente::global))
    if(!contexto.eval(&v)) {
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
  if(!lRotulos.empty()) {
    for(str_it=lRotulos.begin(); str_it!=lRotulos.end(); str_it++)
      resp+=(*str_it) + ", ";
    resp[resp.length()-2] = ':';
  }
  
  // Adiciona o padrao na resposta:
  resp += padrao.str();

  // Adiciona o contexto na resposta:
  if(!contexto.empty()) {
    resp += " - ";
    resp += contexto.str();
  }
  
  if(!lSignificado.empty()) {
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
  for(int level = def_level; level>=0 && bMatch==false; level--) {
    for(auto& i : instrucoes[level]) {
      // Se houver match:
      if(i.second.match(texto)) {
        // Marque que houve match:
        bMatch = true;
        // Guarde a instrução que recebeu o match:
        inst.push_back(i.second);
        // E colete a variável associada (que contem as interpretacoes):
        escopo.push_back(i.second.var);
        // Incremente o contador de matchs:
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
    vector<pair<pMatch::tInterpretacao*, cInst*>> index;
    
    j=0;
    int escolha=0;
    cout << "Ambiguidades encontradas: " << endl << endl;

      cout << "    0 »» cancel\n" << endl;
    
    for(j=0; (unsigned)j<inst.size(); j++) {
      if(escopo[j].lInt.size()==1 && escopo[j].lInt.front().var.size()==0) {
        index.push_back( {&escopo[j].lInt.front(), &(inst[j])} );
        cout << "    " << ++escolha << " »» " << inst[j].str() << endl;
      } else {
        cout << "  " << inst[j].str() << endl;
        
        for(auto& k : escopo[j].lInt) {
          index.push_back ( {&k, &(inst[j])} );
          cout << "    " << ++escolha << " »» " << lVar_to_str(k.var) << endl;
        }
      }
      cout << endl;
    }
    
    string line;
    
    // Now wait for the user answer:
    do {
      getline(cin, line);
      
      escolha = atoi(line.c_str());
    } while ((unsigned)escolha > index.size());

    if(!escolha) {
      cout << "  «« canceled »»" << endl;
    }
    else {
      escolha--;
    
      // E guardamos a interpretação escolhida por ele.
      escopo_escolhido = *(index[escolha].first);
      inst_escolhida = *(index[escolha].second);
    }
  } else {
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
      isBlank.ifind(inst, antes);
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
      isBlank.ifind(inst, antes);
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
      
      isBlank.ifind(inst, antes);
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
      isBlank.ifind(rotulo, pos1);
      
      // Encontre o próximo separador do rotulo:
      mRotulo.find(rotulo, pos2=pos1);
      
      str = string("");
      
      // Coleto o nome do rotulo removendo espaços repetidos:
      for(; pos1<pos2; pos1++)
      {
        if(!isBlank.imatch(rotulo[pos1]))
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
    isBlank.ifind(padrao,ini=0);
    
    // Descarte os espaços no final:
    for(fim=padrao.length()-1; !isBlank.imatch(padrao[fim]); fim--);
    
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
    for(fpos=contexto.length()-1; !isBlank.imatch(contexto[fpos]); fpos--);
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
    isBlank.ifind(significado,i=pos);
    
    // Se houver uma declaração de driver:
    if(significado[i]=='#' && significado[i+1]=='!')
    {
      // Encontra o inicio do nome do driver:
      isBlank.ifind(significado, i+=2); ini=i;
      
      // Encontra o fim do nome do driver:
      while(isBlank.imatch(significado, i) && significado[i]!=':') i++;
      
      // Grava o driver na classe:
      this->driver = significado.substr(ini, i-(ini));
      
      // Encontra o ':':
      if(significado[i]!=':')
        isBlank.ifind(significado, ++i);
      
      // Encontra o inicio do texto:
      isBlank.ifind(significado, ++i);
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
        if( isBlank.imatch(significado[fimTexto]) )
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
          if(!isBlank.imatch(significado[i])) continue;
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
        isBlank.ifind(significado, i);
        // E quebre o loop:
        break;
      }
      else if(i==fimTexto+1)
      {
        // Encontre o ';' final (se houver):
        isBlank.ifind(significado, i);
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

  std::string cSignificado::str() {
    using namespace std;
    
    int aux=0;
    string resp = string("");
    string txt = this->texto;
    list<pair<string,int>>::iterator it;
    
    if(driver != string(""))
      resp+= "#!" + driver + ": ";
    
    resp+='"';
    
    for(it=variaveis.begin(); it!=variaveis.end(); it++)
    {
      txt.insert(it->second+aux, '@'+it->first+';');
      aux+=it->first.length()+2;
    }
    
    resp += txt;
    
    resp+='"';
    resp+=';';
    
    return resp;
  }
  
  void cInst::buildSignificado(string significado)
  {
    int i;
    for(i=0; (unsigned)i<significado.length(); )
    {
      // Caso não hajam mais letras até o fim da string:
      if(isBlank.ifind(significado,i)=='\0') break;
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
  pMatch::charClass cInst::mContexto    = pMatch::charClass("[^][a-zA-Z0-9._ \t\n]");
  pMatch::charClass cInst::isBlank      = pMatch::charClass("[ \t\n]");

  cInst::cInst() {}

  cInst::cInst(const char* inst, int ID) : cInst(string(inst),ID) {}
  cInst::cInst(string inst, int ID)
  {
    this->ID = ID;
    
    validate(inst);
    build(inst);
  }
  
/* * * * * END cInst * * * * */

// Valida uma referencia à variável no formato: "a.b.c[5][10].d"
// Com a possibilidade de aceitar espaços entre os termos com letras e numeros.
// TODO: mover esse validate addr para dentro da classe cExpressao
void validate_addr(std::string contexto, int& pos)
{
  bool bArray=false;
  static charClass isName("[a-zA-Z0-9_]");
  
  // Ignore quaisquer caracteres em branco:
  isBlank.ifind(contexto,pos);
  
  // Se o primeiro caractere não for válido:
  if(!charClass("[a-zA-Z_]").match(contexto[pos]))
    throw "Nome de variável iniciado com caractere inválido!, use apenas [a-zA-Z_]: validate_addr()";

  // Inicie a validação do nome, cada loop corresponde a um item
  // na arvore de variáveis do tipo: "a.b.c[5][10].d"
  while(contexto[pos])
  {
    string var;
    
    // Verifico se a variável é do tipo de acesso à array: '[0]' ou '[100]'
    if(!bArray)
    {
      // Elimine espaços em branco antes do nome da variável.
      while(isBlank.match(contexto[pos])) pos++;
      
      // Leia todas as letras do nome da variável.
      while(isName.match(contexto[pos])) pos++;
      
      // Tratando o fim do nome de um item do endereço de variável
      // do tipo da variável c ou d no exemplo a seguir: "a.b.c.d[10]":
      if(charClass("[[.;, \t\n=<>]").match(contexto[pos]) || !contexto[pos])
      {
        // Encontre a próxima posição que não seja branca.
        if(contexto[pos]==' ') isBlank.ifind(contexto,pos);
        
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
      isBlank.ifind(contexto,pos);
      
      // Leia o número até o final:
      charClass("[^0-9]").find(contexto,pos);
      
      // Despreze os demais caracteres em branco:
      isBlank.ifind(contexto,pos);
      
      if(contexto[pos]!=']')
        throw "Esperado ']' no endereço de variável!: cContexto::validate()";
      else pos++;
      
      // Despreze quaisquer caracteres em branco:
      isBlank.ifind(contexto,pos);
      
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

void cExpressao::build_repr(const std::string& str) {

  this->repr = std::string();
  bool lastblank = true;

  for(char c : str) {
    if(lastblank && isblank(c)) continue;
    lastblank = false;

    if(isblank(c)) lastblank = true;

    this->repr.push_back(c);
  }

  if(isblank(repr.back())) repr.pop_back();
}

cExpressao::cExpressao() : exp() { exp.scope = ambiente::global; }
cExpressao::cExpressao(std::string exp_s, int& pos,
    const std::string& delim, Scope scope) : exp() {
  exp.scope = ambiente::global;

  // Slice out the first part of the string:
  int end = exp_s.find_first_of(delim, pos);
  if(end == -1) end = exp_s.length();

  std::string sub = exp_s.substr(pos, end-pos);

  exp.compile(sub.c_str(), scope);
  build_repr(sub);
  if(this->repr.size() == 0) {
    throw std::string(
        "cExpressao::cExpressao: Invalid empty expression on context!: `" + exp_s + "`.");
  }

  pos = end;
}

void cContexto::build(std::string contexto) {
  int pos=0;
  
  // Para cada item do contexto: " a = b; b < 3; c > 4 "
  while(contexto[pos]) {
    expList.push_back(cExpressao(contexto, pos, ";,"));
    
    if(contexto[pos] == '\0') return;
    else ++pos;
  }
}

// Avalia cada uma das expressões do contexto.
// Caso uma delas seja falsa o contexto é considerado falso.
bool cContexto::eval(Scope scope) {
  for(auto& exp : this->expList) {
    packToken p = exp.eval(scope);
    if(!p.asBool()) return false;
  }
  return true;
}

cContexto::cContexto(std::string contexto) {
  build(contexto);
}

cContexto::cContexto(std::string contexto, int& pos) {
  int inicio = pos;

  if((unsigned)pos >= contexto.size()) {
    throw std::string("cContexto::cContexto: Invalid empty context!");
  }

  // Encontre o fim do contexto:
  while((contexto[pos]!='=' || contexto[pos+1]!='>') && contexto[pos]) ++pos;
  
  contexto = contexto.substr(inicio, pos-inicio);
  
  build(contexto);
}

void executaSig(Scope local, cSignificado sig) {
  int aux=0;
  string str = sig.texto;
  
  for(auto var = sig.variaveis.rbegin(); var!=sig.variaveis.rend(); var++) {
    // Pegue o valor da variável:
    packToken t = local.find(var->first);
    string str_var = t.str();
    
    str.insert(var->second+aux, str_var);
    aux+=str_var.length();
  }
  
  if(sig.driver.length()>0)
    ambiente::drivers[sig.driver]->run(str);
  
  else
    banco::execInst(str);
}

vars::cObject local;

void executa(cInst inst, pMatch::tInterpretacao escopo_local) {
  // Variáveis locais:
  local = vars::cObject(escopo_local);

  list<cSignificado> sigs = inst.getSignificados();
  
  for(auto& sig : sigs)
    executaSig(&local, sig);
}


