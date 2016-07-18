#include "banco.hpp"
#include "ambiente.hpp"
#include <cstdlib>
#include <sstream>
#include <cstring>
#include <sstream>

using namespace std;
using namespace pMatch;

// charClasses utilizados para lidar com caracteres em branco:
charClass isBlank = charClass("[ \t\n]");

// Last id é o último ID adicionado no banco.
// Valor inicial = 0.
uint banco::lastID=0;

// O nível default de adição de novas instruções.
// O padrão é 1, mas isso pode mudar.
int banco::def_level=1;

// TODO: tirar essa limitação do vetor de níveis.
// Vetor de níveis atualmente limitado ao nível [10]
vector<map<uint, cInst>> banco::instrucoes = vector<map<uint,cInst>>(11);

std::map<std::string, cRotulo> banco::rotulos;

/* * * * * START banco * * * * */

cInst* banco::addInst(string inst, int level) {
  uint ID = getUniqueID();
  instrucoes[level][ID] = cInst(inst, ID);
  return &(instrucoes[level][ID]);
}

cInst* banco::addInst(string inst) {
  uint ID = getUniqueID();
  instrucoes[def_level][ID] = cInst(inst, ID);
  return &(instrucoes[def_level][ID]);
}

pair<cInst*, uint> banco::getInst(uint ID) {
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

void banco::remInst(uint ID)
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
  for(uint i=0; i<instrucoes.size(); i++) {
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
  for(uint i=0; i<instrucoes.size(); i++) {
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
  
  uint num_validos=0;
  bool bMatch = this->padrao.match(str, 0, true);
  this->var = padrao.var;
  
  if(!bMatch) return false;
  
  // Para cada interpretação possível, verifique se o contexto é válido:
  for(auto j=var.lInt.begin(); j!= var.lInt.end(); ) {
    auto v = vars::cObject(*j);
    // Se o contexto não for válido:
    if(!contexto.eval(&v)) {
      j = var.lInt.erase(j);
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

std::string cInst::str() {
  using namespace std;
  string resp = string("");
  list<string>::iterator str_it;
  list<cSignificado>::iterator sig_it;
  
  // Adiciona os rotulos na resposta:
  if (!lRotulos.empty()) {
    for(str_it=lRotulos.begin(); str_it!=lRotulos.end(); str_it++)
      resp+=(*str_it) + ", ";
    resp[resp.length()-2] = ':';
  }
  
  // Adiciona o padrao na resposta:
  resp += padrao.str();

  // Adiciona o contexto na resposta:
  if (!contexto.empty()) {
    resp += " - ";
    resp += contexto.str();
  }
  
  if (significado.size() > 0) {
    resp += " ";
    resp += significado.str();
  }
  
  return resp;
}


// TODO: mover essa função para algum lugar adequado.
// Essa função transforma uma lista de variáveis em uma string.
string lVar_to_str(pMatch::lVar vars) {
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
  
  uint j;
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
    uint escolha=0;
    cout << "Ambiguidades encontradas: " << endl << endl;

      cout << "    0 »» cancel\n" << endl;
    
    for(j=0; j<inst.size(); j++) {
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
    } while (escolha > index.size());

    if(!escolha) {
      cout << "  «« canceled »»" << endl;
    } else {
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

bool cRotulo::match(string str, uint pos)
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
  
  #define isvariablestart(c) (isalpha(c) || c == '_')
  void cInst::buildRotulo(string rotulo, uint* fim) {
    uint pos=0;
    std::string str;
    LabelList_t labels;

    // Descarte os caracteres em branco ao inicio:
    while (isblank(rotulo[pos])) ++pos;

    // If the first characters are from a matcher:
    if (rotulo[pos] == '"' || rotulo[pos] == '\'') {
      *fim = pos;
    } else {
      // Look for labels:
      if (!isvariablestart(rotulo[pos])) {
        throw "cInst::buildRotulo: Unexpected character at beggining of instruction!";
      }

      while (true) {
        stringstream ss;
        ss << rotulo[pos];
        ++pos;
        while (rotulo[pos] && (isalnum(rotulo[pos]) || rotulo[pos] == '_')) {
          ss << rotulo[pos];
          ++pos;
        }

        str = ss.str();

        // Save the new label
        labels.push_back(str);

        // Ignore blank characters:
        while (isblank(rotulo[pos])) ++pos;

        // If its the end:
        if (rotulo[pos] == ':') break;

        // If it has more labels:
        if (rotulo[pos] == ',') {
          ++pos;
          // Descarte os caracteres em branco:
          while (isblank(rotulo[pos])) ++pos;
          if (!isvariablestart(rotulo[pos])) {
            throw "Unexpected character on label list!";
          }
        } else {
          throw "Unexpected character on label list!";
        }
      }

      // Link this instruction with each label:
      for (const std::string& str : labels) {
        // Associo essa instrução ao rotulo correspondente na lista de rotulos:
        if (!banco::rotulos.count(str)) {
          banco::rotulos[str] = cRotulo(str);
        }
        banco::rotulos[str].addInst(this->ID);
      }
      // Save the label list:
      this->lRotulos = labels;
      *fim = pos+1;
    }
  }
  
  cSignificado::cSignificado(const char* str, uint& pos) : cSignificado(string(str), pos) {}
  cSignificado::cSignificado(string code, uint& pos) {
    compile(code, pos);
  }

  void cSignificado::compile(const char* code, uint& pos) {
    compile(string(code), pos);
  }

  void cSignificado::compile(string code, uint& pos) {
    raw_code = code.substr(pos);
    const char* p_pos = code.c_str() + pos;
    bool bracket;

    // Find the start of the code block:
    while (*p_pos && *p_pos != '{') ++p_pos;

    bracket = (*p_pos == '{');
    if (bracket) ++p_pos;

    parser.compile(p_pos, &p_pos);
    pos += p_pos-code.c_str();

    // Skip the '}' character:
    if (bracket) {
      if (*p_pos != '}') {
        throw "cSignificado::compile: Expected '}' on end of code block!";
      } else {
        ++pos;
      }
    }
  }

  void cSignificado::exec(const Scope& scope) {
    parser.exec(scope);
  }

  std::string cSignificado::str() {
    return raw_code;
  }
  
  void cInst::build(const std::string& inst) {
    uint pos=0;

    buildRotulo(inst, &pos);

    this->padrao = pMatch::arrayClass(inst, &pos);

    while (isblank(inst[pos])) ++pos;

    if (inst[pos] == '-') {
      ++pos;
      this->contexto = cContexto(inst, pos);
    }

    if (inst[pos] == '{') {
      this->significado = cSignificado(inst, pos);
    }

    if (lRotulos.size() == 0 && significado.size() == 0 ) {
      throw "cInst::cInst: An instruction must have either a label or a code block.";
    }
  }
  
  pMatch::charClass cInst::mRotulo      = pMatch::charClass("[:,{-]");
  pMatch::charClass cInst::mPadrao      = pMatch::charClass("[{-]");
  // TODO: Adicionar o operador @ para o contexto:
  // TODO: Adicionar operadores < > = para o contexto:
  pMatch::charClass cInst::mSignificado = pMatch::charClass(";");
  pMatch::charClass cInst::mContexto    = pMatch::charClass("[^][a-zA-Z0-9._ \t\n]");
  pMatch::charClass cInst::isBlank      = pMatch::charClass("[ \t\n]");

  cInst::cInst() {}

  cInst::cInst(const char* inst, uint ID) : cInst(
    std::string(inst),
    ID) {}
  cInst::cInst(const string& inst, uint ID)
  {
    this->ID = ID;
    build(inst);
  }
  
/* * * * * END cInst * * * * */

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

cExpressao::cExpressao() : exp() {}
cExpressao::cExpressao(std::string exp_s, uint& pos,
    const std::string& delim, Scope scope) : exp() {

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

void cContexto::build(std::string contexto, uint* fim = NULL) {
  uint pos = fim ? *fim : 0;
  
  // Para cada item do contexto: " a = b; b < 3; c > 4 "
  while(contexto[pos]) {
    expList.push_back(cExpressao(contexto, pos, ";,{"));
    
    if(contexto[pos] == '\0' || contexto[pos] == '{') break;
    else ++pos;
  }

  if(fim) *fim = pos;
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

cContexto::cContexto(std::string contexto, uint& pos) {
  if(pos >= contexto.size()) {
    throw std::string("cContexto::cContexto: Invalid empty context!");
  }

  while (isblank(contexto[pos])) ++pos;

  // If there is no context there is nothing to be done:
  if (contexto[pos] == '\0' || contexto[pos] == '{') return;

  build(contexto, &pos);
}

void executa(cInst inst, pMatch::tInterpretacao escopo_local) {
  // Variáveis locais:
  vars::cObject local = vars::cObject(escopo_local);
  inst.exec(&local);
}
