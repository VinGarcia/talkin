#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "padrao.hpp"
#include "vars.hpp"
#include "shunting-yard.h"  // for `calculate` class
#include "code-parser.h"    // for CodeParser class

#ifndef BANCO_DE_INSTRUCOES
#define BANCO_DE_INSTRUCOES

  class cInst;
  class cRotulo;
  
  // Ainda não totalmente testado!
  class banco {
   private:
    banco(){};
    static uint lastID;
    static int def_level;
    
   public:
    typedef std::list<std::string> StrList_t;
    static std::vector<std::map<uint, cInst>> instrucoes;
    static std::map<std::string, cRotulo> rotulos;
    
    static uint getUniqueID()
    {
      static uint nextID=0;
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
    static void remInst(uint ID);
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
    static std::pair<cInst*, uint> getInst(uint ID);
    
    // CONSIDERADO TESTADO
    static std::string str();
    static StrList_t strList();
    static StrList_t rotList();
  };
  
  // Ainda não testado!
  // É preciso que ele seja um matcher pois retorna multiplos significados.
  // e para tanto precisa preencher sua variável de escopo e suas matchWord`s.
  class cRotulo : public pMatch::matcher {
    public:
    std::string nome;
    std::list<uint> instID;
    
    // Simples d+ portanto não foi testado:
    cRotulo() {}
    cRotulo(std::string nome) { this->nome=nome; }
    
    // Simples d+ portanto não foi testado:
    void addInst(uint reference) { this->instID.push_back(reference); }
    
    bool match(std::string, uint pos);
    
    std::string str() {
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
    // Keep a readable representation of the
    // expression. (not on RPN) for str() function.
    std::string repr;
    calculator exp;

    void build_repr(const std::string& str);
  public:
    cExpressao();

    // Used to parse out a expression from a string.
    cExpressao(std::string exp, uint& pos,
        const std::string& delim = ";,", Scope scope = Scope::empty);

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

    void build(std::string, uint* fim);
    
    public:
    cContexto(){}
    cContexto(std::string);
    cContexto(std::string, uint& pos);
    
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
  class cSignificado {
    // Keep the compiled code:
    CodeParser parser;

    // Used for str()
    std::string raw_code;

   public:
    cSignificado() {}
    cSignificado(const char* significado, uint& pos);
    cSignificado(std::string significado, uint& pos);

    void compile(const char* significado, uint& pos);
    void compile(std::string significado, uint& pos);

    uint size() { return parser.size(); }

    void exec(const Scope& scope);
    
    std::string str();
  };
  
  // Testado!
  class cInst {
    uint ID;
    
    // Não utilizados ainda. TODO: verificar se serão utilizados.
    //instt prioridade=1;
    //bool execOnce=false;
    
    public:
    pMatch::arrayClass padrao;
    
// TODO: remover a lista abaixo, ela não está sendo utilizada.
//       é preciso remover suas referencias da construtora antes.
// NOTE: A lista que deveria ser removida é referencia pela função str.
    // Lista de categorizações:
    typedef std::list<std::string> LabelList_t;
    LabelList_t lRotulos;
    
    // Lista de guardas:
    cContexto contexto;
    
    // Lista de significados:
    cSignificado significado;
    
    public:
    pMatch::cVar var;

    void exec(const Scope& scope) { significado.exec(scope); }
    
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
    
    void buildRotulo(std::string rotulo, uint* fim);
    void build(const std::string& inst);
    
    public:
    cInst();
    cInst(const char* inst, uint ID);
    cInst(const std::string& inst, uint ID);
    //class: padrao - contexto { significado }
    
    bool match(std::string texto);
    
    std::string str();
  };

// * * * * * Funções de uso das instruções: * * * * *
  
  void executa(cInst inst, pMatch::tInterpretacao);

#endif
