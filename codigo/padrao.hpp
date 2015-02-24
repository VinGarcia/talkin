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
  // Tested
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
  // Considered tested for extensive use on other modules.
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
        throw "Invalid parameter `pos` on pMatch::tWord::tWord(string,int)";
      this->_start = start;
      this->_end   = start+this->length();
    }
  };
  
  // Considered tested for extensive use on other modules.
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
  
  // List of variables:
  typedef std::list<cVar> lVar;
  
  // Considered tested for extensive use on other modules.
  struct tInterpretacao
  {
    // Each tInterpretacao contains local variables on its values.
    lVar var;
    // Each tInterpretacao contains a text interval that have been interpretated.
    tWord word;
  };
  
  // Class that represents a variable.
  // Considered tested for extensive use on other modules.
  class cVar
  {
    public:
    // Variable name:
    std::string nome;
    
    // List of tInterpretacao objects associated with this variable.
    std::list<tInterpretacao> lInt;
    
    public:
    cVar() {}
    // Considered tested for extensive use on other modules.
    cVar(std::string nome) {this->nome=nome;}
    // Considered tested for extensive use on other modules.
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
  // Considered tested for extensive use on other modules.
  class matcher
  {
    public:

    // Scope variable,
    // keeps track of other variables declared inside this matcher pattern.
    cVar var;
    lWord match_word;
    
    public:
    // Returns the last string matched by the match() function.
    lWord getMatch(){return this->match_word;}
    virtual std::string str()=0;
    
    virtual bool  match   (std::string input, int pos)=0;
    //virtual tWord find    (std::string input, int pos)=0;
    //virtual tWord getClass(std::string, int pos) = 0;
    ~matcher(){}
  };
  
  // Tested
  class strClass : public std::list<charClass>, public matcher
  {
    public:
    strClass(){}
    strClass(const char* str);
    strClass(std::string str);
    
    bool match(std::string input, int pos);
    tWord find(std::string input, int pos);
    
    // The getClass function reads a string until it finds a '\0'
    // or a '(' not preceded by a '\\'.
    // strClass::getClass treat '(' as a special character.
    // The other functions on this class are indiferent to '('.
    static strClass getClass(std::string format, int& pos);
    
    std::string str();
  };
  
  // An arrayClass contains a sequence of strClass`s and blockClass`s
  // A match is obtained if the all the items in the sequence match the
  // given text in the order they are given.
  // Tested
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
    
    // Decode a string into an arrayClass.
    // The string received must be between quotes.
    // `pos` must point to the open-quote before execution
    // and to the close-quote after the execution.
    arrayClass(std::string str, int& pos);
    
    bool match(std::string input, int pos);
    bool match(std::string input, int pos, bool raiz);
    tWord find(std::string input, int pos);
    
    std::string str();
  };
  
  // A blockClass contains a set of arrayClass`s and objectClass`s
  // A match is obtained if any of the items on the set match the whole text.
  // Tested
  class blockClass : public matcher
  {
    bool repeater=false;
    // A vector with the set of child matcher objects.
    std::vector<matcher*> lista;
    
    // Fills the 'lista' array.
    void leLista(std::string str);
    
    // Validate the parameters of the constructor function.
    void validate(std::string str, int fim);
    
    void build(std::string str, int fim);
    
    std::string block_name;
     
    // Recursive step of the blockClass::match() function.
    std::list<tInterpretacao> sub_match(std::string str, int pos, int rep_pos);

    public:
    std::vector<matcher*> getLista(){ return this->lista; }
    std::string nome(){ return block_name; }
    
    public:
    blockClass(){};
    blockClass(const char* str);
    blockClass(std::string str);
    blockClass(const blockClass& block){(*this)=block;}
    
    // This constructor decode a string into a blockClass object.
    // An error is thrown if the conversion is not possible.
    // The string must start with a '(' and end on a ')'.
    blockClass(std::string input, int& pos);
    
    bool match(std::string input, int pos);
    tWord find(std::string input, int pos);
    
    std::string str();
  };
  
  // Not tested yet!
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
    // TODO: make the code for the find function.
    tWord find(std::string, int pos){ return tWord("",0); }
   
    std::string str(){ return *this; }
  };
}

#endif
