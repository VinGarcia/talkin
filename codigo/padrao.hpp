#include <iostream>
#include <sstream>
#include <list>
#include <vector>
#include <string>
#include <map>

#ifndef CASAMENTO_PADRAO
#define CASAMENTO_PADRAO

namespace pMatch
{
  // Testado
  class charClass : public std::string
  {
    public:
    charClass(std::string format);
    charClass(const char* format);
    
    bool match(char input);
    bool match(std::string, int pos);
    char find(std::string input, int& pos);
    static charClass getClass(std::string format, int& pos);
    
    std::string str();
  };
  
  // Not going to be tested explicitly
  // Considerado testado por uso extremo em outros modulos
  class tWord : public std::string
  {
    int _start;
    int _end;
    
    public:
    int start(){return _start;}
    int end(){return _end;}
    
    tWord() : std::string() {}
    
    tWord(const char* word, int start=0) : tWord(std::string(word), start) {}
    
    tWord(std::string word, int start=0) : std::string(word)
    {
      if(start<0)
        throw "Parâmetro pos inválido em pMatch::tWord::tWord(string,int)";
      this->_start = start;
      this->_end   = start+this->length();
    }
  };
  
  // Considerado testado por uso extremo em outros modulos
  class lWord : public std::list<tWord>
  {
    public:
    lWord() : std::list<tWord>() {}
    std::string str()
    {
      std::string resp = std::string("");
      if(this->empty()) return resp; 
      
      std::list<tWord>::iterator it = this->begin();
      resp += (*it); it++;
      for(;it!=this->end(); it++)
        resp += "|" + (*it);
      return resp;
    }
  };
  
  class cVar;
  
  // Lista de variáveis:
  typedef std::list<cVar> lVar;
  
  // Considerado testado por uso extenso em outros modulos.
  struct tInterpretacao
  {
    // Cada interpretação contém variáveis locais com seus valores:
    lVar var;
    // Cada interpretação contém um intervalo de texto que foi interpretado.
    tWord word;
  };
  
  // Classe que representa uma variável.
  // Considerado testado por uso estenso em outros modulos.
  class cVar
  {
    public:
    // Nome da variável:
    std::string nome;
    
    // Lista de interpretações do texto lido por essa variável:
    std::list<tInterpretacao> lInt;
    
    public:
    cVar() {}
    // Considerado testado por uso estenso em outros modulos.
    cVar(std::string nome) {this->nome=nome;}
    // Considerado testado por uso estenso em outros modulos.
    cVar(std::string nome, std::list<tInterpretacao>::iterator it)
    { this->nome=nome; lInt.push_back(*it); }
    
    std::string str(){
      using namespace std;
      string words, resp;
      list<tInterpretacao>::iterator itI=lInt.begin();
      list<cVar>::iterator itV;
      if(itI != lInt.end())
      {
        words += lInt.front().word;
        if(!itI->var.empty())
        {
          words += "{";
          for(itV=itI->var.begin(); itV != itI->var.end(); itV++)
            words += itV->str() + "; ";
          words.pop_back();
          words += "}";
        }
        itI++;
      }
      
      for(; itI != lInt.end(); itI++)
      {
        words += "|" + itI->word;
        if(!itI->var.empty())
        {
          words += "{";
          for(itV=itI->var.begin(); itV != itI->var.end(); itV++)
            words += itV->str() + "; ";
          words.pop_back();
          words += "}";
        }
      }
      
      if(this->nome == "") resp = "no_name: "; else resp = this->nome + ": ";
      resp += words;
      
      return resp;
    }
  };
  
  // Not going to be tested
  // Considerado testado por uso estenso em outros modulos.
  class matcher
  {
    public:
    // Variável de escopo:
    cVar var;
    lWord match_word;
    
    public:
    // Retorna a última string encontrada pelo match();
    lWord getMatch(){return this->match_word;}
    virtual std::string str()=0;
    
    virtual bool  match   (std::string input, int pos)=0;
    //virtual tWord find    (std::string input, int pos)=0;
    //virtual tWord getClass(std::string, int pos) = 0;
    ~matcher(){}
  };
  
  // Testado
  class strClass : public std::list<charClass>, public matcher
  {
    public:
    strClass(){}
    strClass(const char* str);
    strClass(std::string str);
    
    bool match(std::string input, int pos);
    tWord find(std::string input, int pos);
    
    // A função getClass lê uma string até encontrar um '\0'
    // ou um '(' que não seja precedido por um '\\'.
    // Seu comportamento trata o '(' de forma especial.
    // As demais funções dessa classe são indiferentes ao '('.
    static strClass getClass(std::string format, int& pos);
    
    std::string str();
  };
  
  // Uma classe array contém uma sequencia de strClasse`s e blockClasse`s
  // match é obtido se o texto der match em todos da sequencia.
  // onde cada parte do texto deve dar match com um da sequencia.
  // Testado
  class arrayClass : public matcher
  {
    std::list<matcher*> lista;
    
    void build(std::string str);
    
    std::list<tInterpretacao>
    sub_match(std::string, int pos, std::list<matcher*>::iterator it);
    
    public:
    arrayClass(){}
    arrayClass(const char* str);
    arrayClass(std::string str);
    arrayClass(const arrayClass& array){(*this)=array;}
    
    // Decodifica uma string tranformando-a em um arrayClass.
    // A string recebida deve estar entre aspas.
    // pos deve apontar para a abertura da aspas antes da execução.
    // e apontará para o fechamento das aspas após a execução.
    arrayClass(std::string str, int& pos);
    
    bool match(std::string input, int pos);
    bool match(std::string input, int pos, bool raiz);
    tWord find(std::string input, int pos);
    
    std::string str();
  };
  
  // Uma classe bloco contém um conjunto de arrayClasse`s e objectClasse`s
  // match é obtido se o texto der match em ao menos um do conjunto.
  // Testado
  class blockClass : public matcher
  {
    bool repeater=false;
    // Vetor com os possíveis padrões da classe bloco.
    std::vector<matcher*> lista;
    
    // Preenche o array 'lista'
    void leLista(std::string str);
    
    // Verifica a validade do parametro das construtoras:
    void validate(std::string str, int fim);
    
    void build(std::string str, int fim);
    
    std::string block_name;
     
    // Função recursiva auxiliar da blockClass::match()
    std::list<tInterpretacao> sub_match(std::string str, int pos, int rep_pos);

    public:
    std::vector<matcher*> getLista(){ return this->lista; }
    std::string nome(){ return block_name; }
    
    public:
    blockClass(){};
    blockClass(const char* str);
    blockClass(std::string str);
    blockClass(const blockClass& block){(*this)=block;}
    
    // Essa construtora percorre uma string e extraí dela um blockClass.
    // Caso não seja possível é lançado um erro.
    // É preciso que a string comece com um '(' e tenha o formato
    // definido pelo blockClass em seus primeiros caracteres.
    blockClass(std::string input, int& pos);
    
    bool match(std::string input, int pos);
    tWord find(std::string input, int pos);
    
    std::string str();
  };
  
  // Ainda não testado!
  class objectClass : public matcher, public std::string
  {
    void validate(std::string);
    void build(std::string);
    
    public:
    objectClass():std::string(){}
    objectClass(const char* nome);
    objectClass(std::string nome);
    objectClass(const objectClass& object){(*this)=object;}
    
    objectClass(std::string nome, int& pos);
    
    bool match(std::string, int pos);
    // TODO: fazer o código do find
    tWord find(std::string, int pos){ return tWord("",0); }
   
    std::string str(){ return *this; }
  };
}

#endif
