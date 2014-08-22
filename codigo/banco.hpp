#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "padrao.hpp"
#include "vars.hpp"

#ifndef BANCO_DE_INSTRUCOES
#define BANCO_DE_INSTRUCOES

  // Guarda as variáveis locais para a função 'executa'
  extern vars::cVar local;

  class cInst;
  class cRotulo;
  
  // Ainda não totalmente testado!
  class banco
  {
    private:
    banco(){};
    static int lastID;
    static int def_level;
    
    public:
    static std::vector<std::map<int, cInst>> instrucoes;
    static std::map<std::string, cRotulo> rotulos;
    
    static int getUniqueID()
    {
      static int nextID=0;
      lastID = nextID;
      return nextID++;
    }
    
    // CONSIDERADO TESTADO (por causa do teste na função addInst(string))
    // Adiciona nova instrução ao nível 'level'
    static cInst* addInst(std::string inst, int level);
    
    // TESTADO
    // Adiciona nova instrução ao nível 'def_level'
    static cInst* addInst(std::string inst);
    
    // TESTADO
    static void remInst(int ID);
    static void remLast();
    
    // Determina o def_level.
    static void setLevel(int level){ banco::def_level=level; }
    static int getLevel(){ return banco::def_level; }
    
    // Executa instrução no nível 'level'
    static void execInst(std::string inst, int level);
    // Executa instrução no nível 'def_level'
    static void execInst(std::string inst);
    
    // Retorna um ponteiro pra instrução referenciada, e um inteiro
    // que indica o level onde essa instrução está armazenada.
    static std::pair<cInst*, int> getInst(int ID);
    
    // CONSIDERADO TESTADO
    static std::string str();
    static std::list<std::string> strList();
    static std::list<std::string> rotList();
  };
  
  // Ainda não testado!
  // É preciso que ele seja um matcher pois retorna multiplos significados.
  // e para tanto precisa preencher sua variável de escopo e suas matchWord`s.
  class cRotulo : public pMatch::matcher
  {
    public:
    std::string nome;
    std::list<int> instID;
    
    // Simples d+ portanto não foi testado:
    cRotulo(){}
    cRotulo(std::string nome){ this->nome=nome; }
    
    // Simples d+ portanto não foi testado:
    void addInst(int reference){ this->instID.push_back(reference); }
    
    bool match(std::string, int pos);
    
    std::string str()
    {
      std::string str;
      std::ostringstream oss;
      for(auto& id : instID)
        oss << id << ", ";
      str = nome + ": " + oss.str();
      str.pop_back();
      str.pop_back();
      return str;
    }
  };
  
  enum eTipo { VAR, STR, NUM, BOOL, OP };

  // Uma expressão será composta por items que incluem:
  // OP - operadores (+,-,*,/,!,==,!=,<=,>=)
  // NUM - Números literais ({[0-9]+.?} | {.[0-9]+} | {[0-9]+.[0-9]+})
  // STR - String ( " string " )
  // VAR - Referência de variável (var1, var2, _var3, etc...)
  typedef std::pair<eTipo,std::string> expItem;
  
  // O struct abaixo armazena uma expressão em notação polonesa reversa.
  class cExpressao : public std::list<expItem>
  {
    // A lista dessa classe contém os operandos e os operadores.
    // A operação deles deve seguir a ordem da notação polonesa reversa.
    // TODO: Atualmente ele só lê expressões do tipo: a + b com 2 operandos.
    //       Ele deve ser capaz de ler expressões genéricas no futuro.

    std::map<std::string,int> opMap = {
      {"!",  3},
      {"*",  2},
      {"&&", 2},
      {"/",  2},
      {"+",  1},
      {"-",  1},
      {"||", 1},
      {"==", 0},
      {"!=", 0},
      {"<",  0},
      {">",  0},
      {"<=", 0},
      {">=", 0},
    };

    // Matcher para identificar operadores:
    pMatch::charClass opClass = pMatch::charClass("[-!*&/+|=!<>]");
    
    expItem getOperando(std::string exp, int& pos);
    expItem getOperador(std::string exp, int& pos);
    void build(std::string exp);
    
    public:
    
    cExpressao(){}
    cExpressao(std::string exp, int& pos);
    
    // Avalia uma expressão e retorna o resultado calculado:
    std::string eval(vars::cVar local, vars::cVar global);
    
    private:
    // Funções que representam os operadores:
    std::string op_equal(std::string left, std::string right);
    std::string op_diff(std::string left, std::string right);
    std::string op_menor(std::string left, std::string right);
    std::string op_maior(std::string left, std::string right);
    std::string op_and(std::string left, std::string right);
    std::string op_or(std::string left, std::string right);
    std::string op_sum(std::string left, std::string right);
    std::string op_sub(std::string left, std::string right);
    std::string op_mul(std::string left, std::string right);
    std::string op_div(std::string left, std::string right);
    std::string op_not(std::string str);

    // Verifica se a string contém um número (inteiro ou real)
    bool numeric(std::string str);

    // Converte um float para uma string:
    std::string float_to_str(double number);
    
    public:
    std::string str_raw() {
      std::string resp;
      for(auto a : *this) {
        //std::cout << a.first << std::endl;
        if(a.first == STR)
          resp += '"' + a.second + "\" ";
        else
          resp += a.second + " ";
      }
      // Remove o último espaço:
      if(this->size()) resp.pop_back();
      return resp;
    }
  };
  
  // Essa classe é responsável por armazenar e trabalhar
  // com o campos de contexto das instruções.
  class cContexto
  {
    // A lista abaixo contém as expressões do contexto,
    // por exemplo: " a = b; b < 3; c > 4 " contém 3 expressões.
    std::list<cExpressao> expList;
    
    // TODO: Remover essa funcao daqui e move-la para o cExpressao.
    // Atualmente ela está solta no .cpp sem associacao com classe.
    //void validate_addr(std::string, int& pos);
    void readOperand(std::string contexto, int& pos);
    void validate(std::string);
    void build(std::string);
    
    public:
    cContexto(){}
    cContexto(std::string);
    cContexto(std::string, int& pos);
    
    // Avalia o contexto com base nas variáveis globais e locais.
    // Retorna true caso o contexto seja verdadeiro segundo as variáveis dadas.
    bool eval(vars::cVar& local, vars::cVar& global);
    
    bool empty(){ return expList.empty(); }
    
    std::string str(){
      std::string resp;
      for(auto a : expList)
        resp += a.str_raw() + "; ";
      // Remove os dois últimos caracteres:
      resp.pop_back();
      resp.pop_back();
      return resp;
    }
  };

  
  
  // Testado!
  class cSignificado
  {
    public:
    std::string driver;
    
    // Lista das variáveis e as posições de inserção de seus valores:
    std::list<std::pair<std::string,int>> variaveis;
    
    std::string texto;
    
    cSignificado(){}
    cSignificado(const char* significado, int& pos);
    cSignificado(std::string significado, int& pos);
    
    std::string str()
    {
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
  };
  
  // Testado!
  class cInst
  {
    int ID;
    
    // Não utilizados ainda. TODO: verificar se serão utilizados.
    //int prioridade=1;
    //bool execOnce=false;
    
    public:
    pMatch::arrayClass padrao;
    
// TODO: remover a lista abaixo, ela não está sendo utilizada.
//       é preciso remover suas referencias da construtora antes.
    // Lista de categorizações:
    std::list<std::string> lRotulos;
    
    // Lista de guardas:
    cContexto contexto;
    
    // Lista de significados:
    std::list<cSignificado> lSignificado;
    
    public:
    pMatch::cVar var;
    
    //pMatch::arrayClass getPadrao(){ return this->padrao; }
    //std::list<std::string> getContextos(){ return this->lContexto; }
    std::list<cSignificado> getSignificados(){ return this->lSignificado; }
    
    private:
    // Abaixo as classes listam os caracteres delimitadores
    // dos campos "rotulo", "padrao" e "contexto"
    static pMatch::charClass mRotulo;
    static pMatch::charClass mPadrao;
    // TODO: Adicionar o operador @ para o contexto:
    // TODO: Adicionar operadores < > = para o contexto:
    static pMatch::charClass mContexto;
    static pMatch::charClass mSignificado;
    static pMatch::charClass notBlank;
    
    // Usados pela build e pela validate:
    
    // Lista de características da instrução:
    bool validated=false;
    bool rotuloOmitido=false;
    bool contextoOmitido=false;
    bool significadoOmitido=false;
    // Lista de posições da classe, usado pelo validate de pelo build:
    short fimRotulo=-1;
    short fimPadrao=-1;
    short fimContexto=-1;
    short fimSignificado=-1;
    
    // TODO: Adicionar a validação do significado no formato:
    // (#!stdout:)? (texto|'''texto'''|"""texto""") ;
    void validate(std::string inst);
    
    void buildRotulo(std::string rotulo);
    void buildPadrao(std::string padrao);
    void buildContexto(std::string contexto);
    void buildSignificado(std::string significado);
    void build(std::string inst);
    
    public:
    cInst();
    cInst(const char* inst, int ID);
    cInst(std::string inst, int ID);
    //class: padrao - contexto => significado
    
    bool match(std::string texto);
    
    std::string str()
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
      }
      // Remove o ultimo espaço:
      resp.pop_back();
      
      return resp;
    }
  };

// * * * * * Funções de uso das instruções: * * * * *
  
  bool avaliaContexto(
    std::map<std::string, vars::cVar> globalVars,
    pMatch::lVar localVars,
    std::list<std::string> contextos
  );
  
  void executa(cInst inst, pMatch::tInterpretacao);

  //void executa(cInst inst, pMatch::tInterpretacao tInt);
  
#endif
