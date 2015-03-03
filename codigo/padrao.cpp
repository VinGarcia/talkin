
#include <iostream>
#include <sstream>
#include "ambiente.hpp"
#include "padrao.hpp"

// The instruction database `banco` is acessed only by the class objectClass.
#include "banco.hpp"
  
  using namespace std;
  using namespace pMatch;

/* * * * * START charClass: * * * * */
  
  // Constructor of the charClass class:
  pMatch::charClass::charClass(string format) : string(format)
  {
    int lenF = format.length();
    
    /* * * * * * * Start the validity tests * * * * * * */

    if(lenF==0) throw "Empty String!";
    
    if(lenF==2 && format[0]=='\\')
    {
      (*this)[0] = (*this)[1];
      this->pop_back();
    }
    // If format isn't simple as "a" or "b"
    else if(lenF!=1 && !(lenF==2 && format[0]=='\\'))
    // Then do the following tests:
    {
      // If the class is empty "[]"
      if(lenF==2) throw "Empty format: \"[]\"";
      // If the class an inverse class and is empty: "[^]"
      if(lenF==3 && format[1]=='^') throw "Empty format: \"[^]\"";
      // If the class don't end with a ']': "[abc"
      if(format[lenF-1] != ']') throw "Invalid format: \"[abc\"";
      // If the class doesn't start with a '[': "abc]"
      if(format[0]!='[') throw "Invalid format: \"abc]\"";
    
      // Checking if its a class or an inverse class:
      // If its a class the start position is 2, if its an inverse class its 3.
      int j = format[1]!='^'?2:3;
      
      // Continue with the tests:
      
      // If the class have the format: "[abc]edf]"
      if(j+1<lenF)
        for( ; format[j+1]; j++)
          if(format[j] == ']') throw "Invalid format: \"[abc]edf]\"";
      // Note that "[]abc]" or "[^]abc]" is an accepted format.
      // (it`s important to make the class include the character ']' in it)
      
      // In case the user writes something ambiguous like: "[a--b]"
      j = format[1]!='^'?3:4;
      if(j+2<lenF)
        for( ; format[j+2]; j++)
          if(format[j-1]=='-' && format[j] =='-')
            throw "Invalid format: \"[a--b]\"";
      // Note that there is no problem with the formats: "[--b]" or "[a--]"
      
      // In case the user writes: "[a-b-c]" or something like that.
      j = format[1]!='^'?4:5;
      if(j+2<lenF)
        for( ; format[j+2]; j++)
          if(format[j-2]=='-' && format[j] =='-')
            throw "Invalid format: \"[a-b-c]\"";
    }
    
    /* * * * * * * End of the validity tests * * * * * * */
  }
  
  pMatch::charClass::charClass(const char* format) :
    charClass(string(format)) {}
  
// Define used for iterate over a character class format:
// ([abc] for example)
#define forClass(c,i) \
  for(i=1; c[i]!=']' || i==1; i++)

// Define used for iterate over an inverse character class format:
// [^abc]
#define forNotClass(c,i) \
  for(i=2; c[i]!=']' || i==2; i++)

// /*
bool pMatch::charClass::match(string input, int pos)
{ return match(input[pos]); }
bool pMatch::charClass::match(char input)
  {
    charClass format = *this;
    
    // Format length:
    int lenF = format.length();
    
    // * * * * * * * Start the matching case for "a", "b" or "c" * * * * * * *
    
    if(format[0] != '[' || lenF == 1)
      return input==format[0];
    
    // * * * * * * * Start the matching case for "[abc]" * * * * * * *
    
    else if(format[1]!='^')
    // When the format is like: "[abc]"
    {
      int j;
      // Iterating over the string format:
      forClass(format, j)
      {
        // In case format is like: "[a-z]"
        if( j != 1 && j != lenF-2 && format[j] == '-')
        {
          int ini, fim;
          if(format[j-1]<format[j+1])
            {ini = j-1; fim = j+1;}
          else
            {ini = j+1; fim = j-1;}
          
          char c=format[ini];
          for(c++ ; c < format[fim]; c++)
            if(input == c) return true;
        }
        // In case format is simple: "[abc]"
        else if( input == format[j] )
          return true;
      }
      return false;
    }
    
    // * * * * * * * Start the matching case for "[^abc]" * * * * * * *
    
    else
    {
      int j;
      // Iterating over the format:
      forNotClass(format, j)
      {
        // In case the format is like: "[^a-z]"
        if( j != 2 && j != lenF-2 && format[j] == '-')
        {
          int ini, fim;
          if(format[j-1]<format[j+1])
            {ini = j-1; fim = j+1;}
          else
            {ini = j+1; fim = j-1;}
          
          char c=format[ini];
          for(c++ ; c < format[fim]; c++)
            if(input == c)
              return false;
        }
        
        // In case the format is like: "[^abc]"
        if( input == format[j])
          return false;
      }
      
      return true;
    }
    
    // * * * * * * * End function * * * * * * *
    
    // If the execution flow gets here:
    throw "Error in the code! On function pMatch::charClass::match()!";
  }
// */

/*
 *
 * Function FindChar
 * @desc - Search in a string `input` for one character
 *         defined in a set of characters;
 *         
 *         The set of characters are defined on this->format
 * 
 * @param - input: string to be searched.
 *        - format: string containing the set of characters.
 *        - pos: variable passed by reference, contains the start
 *               position of the search on the beginning of the execution.
 *               On the end of the execution it contains the position
 *               of the first matching character.
 *
 * @return - returns the matched character.
 *
 * @notas - the system accepts character classes.
 *          the following formats are accepted:
 *          
 *            "[abc]", "[a-x]", "[x-a]", "[abc-]", "[]]", "[]abc]"
 *          
 *          The inverse format of the above classes are also accepted:
 *
 *            "[^abc], "[^a-x]", "[^x-a]", "[^abc-]", "[^]]", "[^]abc]"
 *
 *          Solo characters are also accepted:
 *
 *            "a", "b", "c" ... etc ...
 *
 *          The meaning of each format is as defined for the GNU RegEx`s
 *          The notation [x-a] also works even if on GNU RegEx`s only [a-x] work.
 *  
 */
char pMatch::charClass::find(string input, int& pos)
{
  charClass format = *this;
  
  int len = input.length();
  int i;
  
  if(pos<0 || pos>len)
    throw "In pMatch::charClass::find() - Parameter 'pos' is invalid";
  
  for(i=pos; input[i]; i++)
  {
    if(format.match(input[i]))
    {
      pos=i;
      return input[i];
    }
  }
  
  pos = i;
  return '\0';
}

/*
 * @name - charClass::getClass
 *
 * @desc - this function extract a the first charClass 
 *         it can find from a string with custom values.
 *
 *         At the end of execution the integer `pos` point to
 *         the first position after the end of the extracted charClass.
 *
 *         This way it is possible to parse a bigger string
 *         into a sequence of charClasses using this function.
 *
 */
// /*
  pMatch::charClass pMatch::charClass::getClass(string format, int& pos)
  {
    int i = pos;
    
    // Treating the parameters:
    if(pos<0 || (unsigned)pos > format.length())
      throw "Invalid parameter `pos`! pMatch::charClass::getClass";
    
    if((unsigned)pos == format.length())
      throw "Empty string! pMatch::charClass::getClass";
    
    // In case there is a character escape:
    if(format[i]=='\\')
    {
      // The answer is the next character of the string:
      charClass resp(format.substr(i+1,1));
      
      // `pos` now points to the first character after the matched one.
      pos+=2;
      
      return resp;
    }
    // In case its found a solo character and not a character class:
    else if(format[i]!='[')
    {
      // Return this charater:
      charClass resp(format.substr(i,1));
      // Point `pos` to the next position:
      pos++;
      return resp;
    }
    
    // If it got here its a character class like: "[abc]" ou "[^abc]"
    
    // Now seach for the ending character: ']'

    // Jump the open brackets and the first character:
    i+=2;
    
    // Note that i+=2 jump the first character of the class.
    // This happens for allowing a class like: "[]abc]" or "[]]"
    
    // And for allowing classes like: "[^]abc]" e "[^]]":
    if(format[i-1]=='^') i++;
    
    if( (unsigned)i < format.length() )
    
      // For each character starting from the `i` position:
      for(; format[i]; i++)
      {
        // If its a closing brackets:
        if(format[i]==']')
        {
          // Build charClass:
          charClass resp(format.substr(pos, i-pos+1));
          // update `pos` to the next position:
          pos = i+1;
          
          return resp;
        }
      }
      // Since we jumped the first character (i+=2)
      // The following classes are accepted: "[]abc]", "[^]abc]"
      // And the following isn't: "[]", "[^]"

    // If the format opens a bracket but doesn't close it:
    throw "Format has an opening '[', but don't end in ']'! pMatch::getClass";
  }
// */

  string pMatch::charClass::str()
  {
    return (*this);
  }

/* * * * * END charClass: * * * * */


/* * * * * START strClass * * * * */

  /*
   * @nome - strClass::strClass()
   * 
   * @desc - Build a std::list of consecutive charClasses.
   *         
   *         Note that it's acceptable an empty list (thus an empty strClass).
   *         
   *         An empty strClass works as a <lambda> on Computer Science Theory
   *         thus matchs any given string as true.
   */
  pMatch::strClass::strClass(string str)
  {
    int pos=0;
    
    // Back slash is not accepted on the end of the string:
    if(str[str.length()-1] == '\\')
      str.pop_back();
    
    // Parse the string using the charClass::getClass
    // Note that pos is received by reference on getClass function.
    while(str[pos])
      this->push_back( pMatch::charClass::getClass(str, pos) );
  }
  
  pMatch::strClass::strClass(const char* str) :
    strClass(string(str)) {}
  
  /*
   * @nome - strClass::match()
   * 
   * @desc - matchs a normal string character by character
   *         with its own charClass list.
   *         return true if all is matched.
   *
   *         note that strClass("") will always return true.
   *
   */
  bool pMatch::strClass::match(string input, int pos)
  {
    // Clean the old match_word list:
    this->match_word.clear();
    
    int len = this->size();
    strClass::iterator it = this->begin();
    
    for(int i=0; i<len; i++, it++)
    {
      if(!it->match(input[pos+i]))
        return false;
    }
    
    // Add a new item to the list and return:
    this->match_word.push_back(tWord(input.substr(pos,len), pos));
    return true;
  }

  /*
   * @nome - strClass::find()
   * 
   * @desc - receive an input string and parse it until the first match.
   * 
   */
  pMatch::tWord pMatch::strClass::find(string input, int pos)
  {
    int lenF = this->size();
    
    // If this format is of an empty strClass:
    if(lenF == 0)
      // TODO: check if returning an empty string is the correct behavior.
      //       and make sure there is a test enforcing the correct behavior.
      return tWord("", pos);
    
    strClass::iterator it=this->begin();
    
    int i=pos;
    
    // Find the first matching character on input:
    (*it).find(input, i); 
    
    while(input[i])
    {
      
      if(this->match(input, i))
      {
        pos=i;
        return tWord(input.substr(i,lenF),i);
      }
      else i++;
      
      // Find the first matching character on input:
      (*it).find(input, i);
    }
    
    // In case it finds nothing:
    return tWord("", i);
  }
  
  /*
   * @name: strClass::getClass()
   *
   * @desc: parse a string searching for the bigger string possible, and returning it.
   *        the string may end in '\0' or '('
   *
   */
  pMatch::strClass pMatch::strClass::getClass(string input, int& pos)
  {
    int start = pos;
    char c;
    
    if(pos<0 || (unsigned)pos > input.length())
      throw "`pos` parameter with invalid value! pMatch::strClass::getClass";
     
    while(1)
    {
      // Search the string for an open parenthesis:
      c = charClass("(").find(input, pos);
      
      // If a parenthesis was found after a back slash:
      if(c=='(' && input[pos-1] == '\\')
      {
        // ignore it
        pos++;
        continue;
      }
      else 
      {
        // Else return the string found:
        return strClass(input.substr(start, pos-start));
      }
    }
  }
  
  // Make a string representing the object content:
  string pMatch::strClass::str()
  {
    strClass::iterator it;
    string resp("");
    
    // For each charClass on the list:
    for(it = this->begin(); it!=this->end(); it++)
      resp += *it;
    return resp;
  }

/* * * * * END strClass * * * * */

/* * * * * START arrayClass * * * * */
  
  void pMatch::arrayClass::build(string str)
  {
    int antes=0;
    int pos=0;
    
    // Posição de inicio do ultimo charClass lido,
    // importante para a construtora do repeater: "[123]*"
    int cc_pos;
    bool in_class;
    
    while( true )
    {
      // Caso encontre um parentesis antes do fim do str:
      if(str[pos]=='(' && (pos==0 || str[pos-1]!='\\'))
      {
        // Se havia algum texto antes do parentesis ainda não adicionado:
        if(antes!=pos)
          lista.push_back(new strClass(str.substr(antes,pos-antes)));
        
        // Como encontrei um bloco,
        // Adiciono o bloco à lista:
        lista.push_back(new blockClass(str, pos));
        // pos é passado por referencia e agora aponta para o próximo
        // caractere desejado.
        
        antes=pos;
        
        if(!str[pos]) break;
        continue;
      }
      
      // Caso encontre um repeater no meio do strClass:
      if(str[pos]=='*' && pos>0 && str[pos-1]!='\\')
      {
        // Se não estava em uma classe, então
        // o repeater terá um único caractere iniciado em pos-1:
        if(!in_class) cc_pos=pos-1;
        // Caso contrário o valor cc_pos será mantido.
        
        // Se havia algum texto antes do cc_pos que não foi adicionado,
        if(antes<cc_pos)
          // Adiciono-o.
          lista.push_back(new strClass(str.substr(antes,cc_pos-antes)));
        
        // Adiciono a classe de caracteres na forma de um repeater à lista:
        // Lembrando que o repeater está implementado como um blockClass.
        lista.push_back(
          new blockClass("(\""+str.substr(cc_pos,pos-cc_pos)+"\")*")
        );
        
        pos++;
        antes=pos;
        
        if(!str[pos]) break;
        continue;
      }
      
      // Se estamos no caractere após a classe,
      // Desativamos o marcador de classe.
      if(in_class && str[pos-1]==']') in_class=false;
      
      // Se encontrar uma classe de caracteres:
      if(!in_class && str[pos]=='[' && (pos==0 || str[pos-1]!='\\'))
      {
        // Anote que econtrou:
        in_class=true;
        // E guarde a posição de inicio:
        cc_pos = pos;
      }
      
      if(str[pos]=='\0')
      {
        // Adicione o resto do str como um strClass:
        lista.push_back(new strClass(str.substr(antes,pos-antes)));
        break;
      }
      
      pos++;
    }
  }

  pMatch::arrayClass::arrayClass(const char* str):arrayClass(string(str)){}
  pMatch::arrayClass::arrayClass(string str): lista()
  {
    build(str);
  }
  
  pMatch::arrayClass::arrayClass(string str, int& pos)
  {
    int fim;
    int pCount=0;
    
    if(str[pos] != '"')
      throw "`str` inválido! pMatch::arrayClass::arrayClass()";
    
    // Encontre o fecha aspas:
    for(fim=pos+1; str[fim]; fim++)
    {
      if(str[fim] == '"' && str[fim-1] != '\\' && pCount==0) break;
      if(str[fim] == '(') pCount++;
      if(str[fim] == ')') pCount--;
    }
    
    if(!str[fim])
    {
      if(pCount==0)
        throw "Faltou um fecha aspas!: pMatch::arrayClass::arrayClass()";
      else
        throw "Faltou um fecha parentesis!: pMatch::arrayClass:arrayClass()";
    }
    
    build(str.substr(pos+1,fim-1-pos));
    
    pos = fim+1;
  }
  
  // Função privada recursiva de matchs do array class.
  // Chamada internamente pela função publica match().
  list<pMatch::tInterpretacao>
  pMatch
  ::arrayClass::sub_match(string str, int pos, list<matcher*>::iterator it)
  {
    int ini=pos, p_atual=pos;
    list<tInterpretacao> lInt;
    list<tInterpretacao>::iterator aux, end;
    
    // O front contém a interpretação deste ramo.
    lInt.push_front({.var=lVar(), .word=tWord()});
    // Os demais itens de lInt conterão as interpretações dos outros
    // ramos dessa recursão.
    
    cVar var;
    
    // Enquanto não acabar a str nem a lista, continue:
    for(; (unsigned)p_atual<str.length() && it != lista.end(); it++)
    {
      // Se não conseguir terminar o match:
      if(!(*it)->match(str,p_atual))
      {
        lInt.pop_front();
        return lInt;
      }
      
      // Caso contrário continue na busca:
      else
      {
        // Coleto a variável associada à este match:
        var = (*it)->var;
        // Se ela tiver nome adiciono-a como parte desse escopo.
        if(var.nome != string(""))
        {
          // Adiciono a nova variável à interpretação:
          lInt.front().var.push_back(cVar(var.nome, var.lInt.begin()));
          
          // Concateno o texto lido com o texto atual:
          lInt.front().word += var.lInt.front().word;
        }
        else
        {
          // Concateno somente o texto:
          lInt.front().word += (*it)->getMatch().front();
        }
        
        // Agora coleto as interpretações ambiguas se existirem:
        aux = ++(var.lInt.begin());
        //aux++;
        end = var.lInt.end();

        // Se houverem interpretações adicionais:
        if(aux != end)
        {
          // Copio a substring que já recebeu match até a p_atual:
          tWord w = tWord(str.substr(ini, p_atual-ini), ini);
          
          list<tInterpretacao> recursion;
          list<tInterpretacao>::iterator it2;
          
          // Faço itProx = it+1:
          it++;
          list<matcher*>::iterator itProx = it;
          it--;
          // E para cada uma delas:
          for(; aux != end; aux++)
          {
            // Faço uma chamada recursiva:
            recursion = sub_match(str, p_atual+(*aux).word.length(), itProx);
            
            // Adiciono w e (*aux).word como prefixo destas interpretações.
            // Bem como adiciono as variáveis antigas a suas variáveis.
            for(it2 = recursion.begin(); it2 != recursion.end(); it2++)
            {
              if(var.nome != string(""))
              {
                list<cVar> var2=lInt.front().var;
                it2->var.push_front(cVar(var.nome, aux));
                it2->var.insert(it2->var.begin(), var2.begin(),--var2.end());
               }
              (*it2).word = tWord(w + (*aux).word + (*it2).word, ini);
            }
            
            // E incluo as novas interpretações na lista lInt:
            lInt.splice(lInt.end(), recursion);
          }
        }
        
        // Atualizo a p_atual:
        p_atual = lInt.front().word.length() + ini;
      }
    }
    
    // Se o algoritmo não tiver terminado as iterações:
    if(it != lista.end())
    {
      // Retorne os casos onde foi possível interpretar algo:
      lInt.pop_front();
      return lInt;
    }
    else
    // TODO: salvar o resto da string na variável rest.
      return lInt;
  }
  
  bool pMatch::arrayClass::match(string str, int pos)
  {
    return match(str, pos, false);
  }
  
  bool pMatch::arrayClass::match(string str, int pos, bool raiz)
  {
    // Limpa as interpretações antigas:
    this->var.lInt.clear();
    this->match_word.clear();
    
    // Lança o matcher recursivo e salva seu retorno.
    this->var.lInt = sub_match(str, pos, this->lista.begin());
    
    // Preenche o match_word
    list<tInterpretacao>::iterator it = this->var.lInt.begin();
    for(; it != this->var.lInt.end(); it++)
      this->match_word.push_back((*it).word);
    
    // Se este array for um array raiz (não está contido em um bloco)
    if(raiz)
    {
      auto i = this->match_word.begin();
      auto j = this->var.lInt.begin();
 
      while( i!=match_word.end() && j!=var.lInt.end() )
      {
        if((*i) != str)
        {
          i=match_word.erase(i);
          j=var.lInt.erase(j);
          continue;
        }
        i++;
        j++;
      }
    }
    
    // Verifica se houve match e retorna:
    if(this->var.lInt.empty()) return false;
    return true;
  }
  
  pMatch::tWord pMatch::arrayClass::find(std::string input, int pos)
  {

/* ignorando a implementação da função.

    bool bMatch=false;
    for(; input[pos]; pos++)
    {
      if(match(input,pos)) bMatch = true;
    }
    
    if(bMatch) return this->match_word;
    
// */
    // Em caso de falha retorne uma string vazia:
    return tWord("", pos);
  }
  
  string pMatch::arrayClass::str()
  {
    list<matcher*>::iterator it;
    string resp = string("");
    
    for(it=lista.begin(); it != lista.end(); it++)
      resp += (*it)->str();
      
    return "\""+resp+"\"";
  }

/* * * * * END arrayClass * * * * */

  
/* * * * * START blockClass * * * * */
  
  // Espera uma string composta por uma lista de strings nos formatos:
  // "<arrayClass_em_formato_string>" e <nome_de_objeto>
  // podendo alternar os dois tipos de formatos sempre separados por ','.
  void pMatch::blockClass::leLista(string str)
  {
    int i=0;
    
    if(str.empty())
      throw "Lista de objetos vazia!: pMatch::blockClass()";
    
    // Até que a lista acabe:
    while(true)
    {
      // Ignore quaisquer espaços em branco iniciais:
      if(str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
      { i++; continue; }

      if(str[i]=='"')
        // Se for um arrayClass:
        lista.push_back(new arrayClass(str, i));
      else
        // Se encontrar um objectClass:
        lista.push_back(new objectClass(str, i));
      
      // Caso tenha-se chegado ao fim da lista retorne:
      if(str[i] == '\0') return;
      else
      {
        while( str[i]!=',' && str[i] != '\0') i++;
        if(str[i] == '\0') return;
        i++;
      }
    }
  }
  
  // Verifica se a string recebida pela construtora é
  // compatível com o formato de um objeto blockClass.
  void pMatch::blockClass::validate(string str, int fecho=0)
  // NOTA: fecho é a posição do fecha parentesis.
  {
    int len = str.length();
    int aux;
    
    // Teste de validade da entrada:
    if(str[0] != '(')
      throw "formato inválido: \"<lista>)<nome>;\": pMatch::blockClass()";
    
    // Se o fecho não for provido previamente:
    if(!fecho){
      for(fecho = len-1; str[fecho] != ')' && fecho!=0; fecho--);
      if(fecho == 0)
        throw "formato inválido: \"(<lista><nome>;\": pMatch::blockClass()";
    }
    
    // Verifique se todos os itens da lista estão separados por virgula
    // e sem caracteres incorretos:
    bool aspas=false, virgula=true, object_name=false;
    int parentesis=0;
    charClass invalChar = charClass("[:()]");
    charClass blank = charClass("[ \n\t]");
    for(int i=1 ; i<fecho; i++)
    {
      if(!object_name && !aspas)
      {
        if(invalChar.match(str[i]))
          throw "caractere inválido na lista do bloco!: pMatch::blockClass()";
        
        if(!virgula)
        {
          if(str[i]==',') virgula = true;
          else if(str[i]!=' ' && str[i]!='\n' && str[i]!='\t')
            throw "formato invalido faltou uma virgula!: pMatch::blockClass()";
        }
        else if(virgula)
        {
          if(str[i]=='"')
            { aspas=true; virgula=false; }
          
          if(str[i]!='"' && str[i]!=' ' && str[i]!='\t' && str[i]!='\n')
            { object_name=true; virgula=false; }
        
          if(str[i]==',')
            throw "formato inválido item vazio na lista!: pMatch::blockClass()";
        }
      }
      else if(object_name)
      {
        if(invalChar.match(str[i]))
          throw "caractere inválido na lista do bloco!: pMatch::blockClass()";
        
        if(str[i] == '"')
          throw "caractere invalido na lista do bloco!: pMatch::blockClass()";
        
        if(str[i]==',') { virgula=true; object_name=false; }
      }
      else if(aspas)
      {
        if(str[i]=='(' && str[i-1]!='\\') parentesis++;
        if(str[i]==')' && str[i-1]!='\\') parentesis--;
        if(parentesis==0 && str[i]=='"' && str[i-1]!='\\')
          aspas=false;
      }
    }
    if(virgula==true)
      throw "formato inválido item vazio na lista!: pMatch::blockClass()";
    
    // Encontre o caractere de fim do nome do objeto:
    charClass("[^a-zA-Z0-9_]").find(str, aux=fecho+1);
    
    // Verifique se ele está na posição final da string recebida:
    if(str[len-1]==' ' || str[len-1] == ';' || str[len-1]=='*')
      if(aux != len-1) throw
        "formato inválido \"(<lista>)<nome_invalido>;\": pMatch::blockClass()";
      else;
    else if(aux != len) throw
        "formato inválido \"(<lista>)<nome_invalido>;\": pMatch::blockClass()";
    // Note que os caracteres de fim aceitos acima são: ' ', ';', '*' e '\0'.
      
    // Verifica se o nome começa com um número (isso é incorreto)
    if( str[fecho+1] && charClass("[0-9]").match(str[fecho+1]) )
      throw "O nome do objeto deve começar com uma letra: pMatch::blockClass()";
    
    // Fim dos testes de validade da entrada.
  }
  
  // (fecho é o indice do fecha parentesis)
  void pMatch::blockClass::build(string str, int fecho)
  {
    int len = str.length();
    
    if(str[len-1]=='*') this->repeater = true;
    
    // Normaliza str para um padrão único:
    if(str[len-1]==' ' || str[len-1]==';' || str[len-1]=='*')
    {
      len--;
      str.pop_back();
    }
    
    // Le a lista de objetos e a salva no array this->lista.
    leLista(str.substr(1,fecho-1));
    
    // Le o nome e o armazena:
    this->block_name = str.substr(fecho+1,len-fecho-1);
    
    // Nomeia a variável:
    this->var.nome = this->block_name;
  }
    
  pMatch::blockClass::blockClass(const char* str):
  blockClass(string(str)){}
  pMatch::blockClass::blockClass(string str)
  {
    int len = str.length();
    
    // Guarda o fim do parentesis:
    int fim;
    // Encontrando o parentesis de fechamento:
    for(fim=len-1; str[fim]!=')'; fim--);
    
    // Teste de validade do parametro:
    validate(str, fim);
    
    // Construção da classe:
    build(str, fim);
  }
  
  pMatch::blockClass::blockClass(string str, int& pos)
  {
    // A posição do fecha parentesis:
    int inicio=pos;
    int fecho;
    int pCount=0;
    string resp;
    
    if(str[inicio]!='(')
      throw "O bloco precisa começar com '('! pMatch::blockClass::blockClass()";
    
    // Encontro o fecha parentesis do bloco:
    for(fecho=inicio+1; str[fecho]; fecho++)
    {
      if(str[fecho]==')' && str[fecho-1]!='\\' && pCount==0) break;
      if(str[fecho]=='(' && str[fecho-1]!='\\') pCount++;
      if(str[fecho]==')' && str[fecho-1]!='\\') pCount--;
    }
    
    if(!str[fecho])
      throw "Faltou um fecha parentesis: pMatch::blockClass(string,int)";
    
    // Encontra o fim da string:
    // (o caractere após o último que será utilizado)
    if( charClass("[ ;(*]").find(str, pos=fecho+1) )
      pos++;
    
    // Se o bloco termina concatenado com outro bloco:
    if(str[pos-1]=='(') pos--;
   
    // Refaz a string e atualiza as variáveis:
    resp = str.substr(inicio,pos-inicio);
    fecho -= inicio;
    
    validate(resp, fecho);
    
    build(resp, fecho);
    
    // Se o caractere de fim era um ' ' decremente pos.
    if(str[pos-1]==' ') pos--;
  }
// /* 
  list<pMatch::tInterpretacao>
  pMatch::blockClass::sub_match(string str, int pos, int rep_num)
  {
    list<tInterpretacao> lInt;
    list<tInterpretacao> lInt_aux;
    vector<matcher*>::iterator it = lista.begin();
    list<tInterpretacao> matched_lInt;
    
    ostringstream oss;
    oss << "[" << rep_num << "]";
    string rep_name = oss.str();
    
    if((unsigned)pos>=str.length()) return lInt;
    
    // Para cada item do bloco:
    for( ; it != lista.end(); it++, rep_num++)
      // Se houver match:
      if((*it)->match(str, pos))
      {
        cVar rep_var;
        
        matched_lInt = (*it)->var.lInt;
        
        // Caso se trate de um repeater:
        if(this->repeater)
        {
          auto it_lInt=matched_lInt.begin();
          for( ; it_lInt != matched_lInt.end(); it_lInt++)
          {
            if((unsigned)pos+it_lInt->word.length() >= str.length()) continue;
            
            // Coleto as interpretações do sub-match:
            // As interpretações para a variavel: [rep_num+1]
            lInt_aux = sub_match(str, pos+it_lInt->word.length(), rep_num+1);
            
            // Concatenando as words novas com as antigas:
            for(auto& i : lInt_aux)
            {
              rep_var = cVar(rep_name, it_lInt);
              
              // Copio as variaveis da lista:
              //rep_var.lInt. i.var.insert(
              //  i.var.begin(), it_lInt->var.begin(), it_lInt->var.end()
              //);
              
              i.var.push_front(rep_var);
              i.word = it_lInt->word + i.word;
            }
            
            lInt.splice(lInt.begin(), lInt_aux);
          }
          
          for(auto i=matched_lInt.begin(); i!=matched_lInt.end(); i++)
          {
            tInterpretacao Int;
            
            Int.var.push_front( cVar(rep_name, i) );
            Int.word = i->word;
            
            lInt.push_front(Int);
          }
        }
        
        if(!repeater)
          lInt.splice(lInt.begin(), matched_lInt);
      }
   
    return lInt;
  }
  
  bool pMatch::blockClass::match(string input, int pos)
  {
    // Limpa a lista de interpretações:
    this->var.lInt.clear();
    this->match_word.clear();
    
    this->var.lInt = sub_match(input, pos, 0);
    
    // Como um repeater deve retornar true, mesmo para 0 repetições:
    if(repeater)
    {
      bool temVazio=false;
      for(auto& i : var.lInt)
        if(i.word==string("")) { temVazio=true; break; }
      
      if(!temVazio)
      {
        tInterpretacao vazio;
        var.lInt.push_front(vazio);
      }
    }
    
    // Preencho o match_word:
    for(auto& i : this->var.lInt)
      this->match_word.push_back(i.word);
    
    if(this->var.lInt.empty()) return false;
    else return true;
  }
  
// */
/* Backup da versão funcional e testada do blockClass::match
   (antes dele também virar um repeater.
   TODO: remover essa versão quando a nova estiver testada.
  bool pMatch::blockClass::match(string input, int pos)
  {
    // Limpa a lista de interpretações:
    this->var.lInt.clear();
    this->match_word.clear();
    
    vector<matcher*>::iterator it = lista.begin();
    
    bool bMatch = false;
    
    // Para cada item do bloco:
    for( ; it != lista.end(); it++)
      // Se houver match:
      if((*it)->match(input, pos))
      {
        bMatch=true;
        
        // Salve as interpretações e as respectivas palavras:
        this->var.lInt.splice  (var.lInt.end()  , (*it)->var.lInt  );
        this->match_word.splice(match_word.end(), (*it)->getMatch());
      }
    
cout << "pos: " << pos << endl;
cout << "var: " << var.str() << endl;
    if(bMatch) return true;
    else return false;
  }
// */
  
  pMatch::tWord pMatch::blockClass::find(string input, int pos)
  {
    int i=pos;
/* ignorando a implementação:
    bool bMatch = false;
    
    vector<matcher*>::iterator it;
    
    for(i=pos; (unsigned)i<input.length(); i++)
      for(it = lista.begin(); it!=lista.end(); it++)
        if((*it)->match(input, i)) bMatch = true;
    
    if(bMatch) return this->match_word;
    
// */
    // Em caso de falha retorne uma string vazia:
    return tWord("",i);
  }
  
  string pMatch::blockClass::str()
  {
    string resp = string("(");
    vector<matcher*>::iterator it = lista.begin();
    
    if(lista.empty())
      throw "blockClass não inicializado!: pMatch::blockClass::str()";
    
    resp += (*it)->str(); it++;
    
    // Para cada item adicional:
    for( ; it != lista.end(); it++)
      resp += "," + (*it)->str();
    
    resp += ")";
    
    if(repeater)
      resp += this->nome() + "*";
    else
      resp += this->nome() + ";";
    
    return resp;
  }

/* * * * * END blockClass * * * * */
  
/* * * * * START objectClass * * * * */

  void pMatch::objectClass::validate(string str)
  {
    int pos=0;
    char c;
    
    // Verifica se há algum caractere inválido no nome:
    c = charClass("[:(),]").find(str,pos);
    if(c) throw "Caractere inválido no nome!: pMatch::objectClass()";
  }
  
  void pMatch::objectClass::build(string str)
  {
    int i=0;
    charClass blank  = charClass("[ \n\t]");
    charClass nblank = charClass("[^ \n\t]");
    
    // Flag blank (se o char lido é branco, fblank = true)
    bool fblank=false;
    
    // Encontre o primeiro caractere não branco.
    nblank.find(str, i);
    
    // Agora leia a string e adicione os caracteres:
    for(; str[i]; i++)
    {
      // Se o caractere encontrado for branco
      // e o último também era:
      if(blank.match(str[i]) && fblank)
        continue;
      
      // Se encontrar um caractere branco:
      if(blank.match(str[i]) && !fblank)
        fblank = true;
      
      // Se encontrar um caractere não branco:
      if(nblank.match(str[i]) && fblank)
        fblank = false;
      
      if(fblank)
        (*this) += ' ';
      else
        (*this) += str[i];
    }
    
    // Se o último caractere inserido era um caractere em branco:
    if(blank.match(str[i-1]))
      // Remova-o
      this->pop_back();
    
    // fim da construção
  }

  // Após a execução pos deve apontar para a posição imediatamente
  // após o objeto.
  pMatch::objectClass::objectClass(string str, int& pos) : string()
  {
    // Marque a posição de inicio:
    int inicio = pos;
    
    // Encontra o final do nome do objeto:
    charClass("[,)]").find(str, pos);
    
    str = str.substr(inicio,pos-inicio);
    
    validate(str);
    build(str);
    
    // Se str[pos] != '\0'
    if(str[pos]) pos++;
  }
  
  pMatch::objectClass::objectClass(const char* str):
  objectClass(string(str)){}
  pMatch::objectClass::objectClass(string str):string()
  {
    validate(str);
    build(str);
  }
  
  bool pMatch::objectClass::match(string str, int pos)
  {
    // Limpa a lista antiga:
    this->var.lInt.clear();
    this->match_word.clear();
    
    if(banco::rotulos.count(*this)==0) return false;
    
    bool resp = banco::rotulos[*this].match(str,pos);
    
    this->var.lInt.splice(var.lInt.end(), banco::rotulos[*this].var.lInt);
    this->match_word.splice(match_word.end(), banco::rotulos[*this].getMatch());
    
    // Apaga os nomes das variáveis para evitar que sejam lidas.
    // (lembrando que o objetos não retornam variaveis internas)
    for(auto& tInt : this->var.lInt)
      for(auto& v : tInt.var)
        v.nome="";
    
    return resp;
  }

/* * * * * END objectClass * * * * */
  
