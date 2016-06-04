#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "padrao.hpp"
#include "vars.hpp"
#include "exp-parser/shunting-yard.h" // for `calculate` class

#ifndef BANCO_DE_INSTRUCOES
#define BANCO_DE_INSTRUCOES

  // Guarda as variáveis locais para a função 'executa'
  // TODO: Mover isso pra dentro da função 'executa'
  extern vars::cObject local;

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
  
  class cExpressao
  {
    // Used to keep a readable representation of the
    // expression. (not on RPN) for str() function.
    std::string repr;
    calculator exp;

    void build_repr(const std::string& str);
  public:
    cExpressao() : exp() {}

    // Used to parse out a expression from a string.
    cExpressao(std::string exp, int& pos,
        const std::string& delim=";,", Scope scope= Scope());

    packToken eval(Scope scope) {
      return exp.eval(scope);
    }
    
  public:
    std::string str() {
      return repr;
    }

    std::string str_raw() {
      return exp.str();
    }
  };
  
  // Essa classe é responsável por armazenar e trabalhar
  // com o campos de contexto das instruções.
  class cContexto
  {
    // A lista abaixo contém as expressões do contexto,
    // por exemplo: " a = b; b < 3; c > 4 " contém 3 expressões.
    std::list<cExpressao> expList;

    void readOperand(std::string contexto, int& pos);
    void validate(std::string);
    void build(std::string);
    
    public:
    cContexto(){}
    cContexto(std::string);
    cContexto(std::string, int& pos);
    
    // Avalia o contexto com base nas variáveis globais e locais.
    // Retorna true caso o contexto seja verdadeiro segundo as variáveis dadas.
    bool eval(Scope local);
    
    bool empty(){ return expList.empty(); }
    
    std::string str(){
      std::string resp;
      for(auto a : expList)
        resp += a.str() + ", ";
      // cout << "a.str_raw(): '" << expList.head().str_raw() <<  "'" << endl;
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
// NOTE: A lista que deveria ser removida é referencia pela função str.
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
    static pMatch::charClass isBlank;
    
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
    // (#!stdout:)? (texto|\'texto\'|\"texto\") ;
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
    
    std::string str();
  };

// * * * * * Funções de uso das instruções: * * * * *
  
  // TODO: Remover esse prototipo:
  // bool avaliaContexto(
  //   std::map<std::string, vars::cObject> globalVars,
  //   pMatch::lVar localVars,
  //   std::list<std::string> contextos
  // );
  
  void executa(cInst inst, pMatch::tInterpretacao);

  //void executa(cInst inst, pMatch::tInterpretacao tInt);
  
#endif
