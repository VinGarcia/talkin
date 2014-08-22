
#include <iostream>
#include <sstream>
#include "ambiente.hpp"
#include "padrao.hpp"

// O banco é acessado somente pela classe objectClass.
#include "banco.hpp"
  
  using namespace std;
  using namespace pMatch;

/* * * * * START charClass: * * * * */
  
// Construtora da Classe de caracteres:
  pMatch::charClass::charClass(string format) : string(format)
  {
    // Length do formato:
    int lenF = format.length();
    
    /* * * * * * * Inicio dos Testes de Validade * * * * * * */
    
    if(lenF==0) throw "String Vazia!";
    
    if(lenF==2 && format[0]=='\\')
    {
      (*this)[0] = (*this)[1];
      this->pop_back();
    }
    // Se o formato não for do tipo simples: "a" ou "b"
    else if(lenF!=1 && !(lenF==2 && format[0]=='\\'))
    // Então faça os seguintes testes de coerência:
    {
      // Caso a classe esteja vazia: "[]"
      if(lenF==2) throw "Formato Vazio: \"[]\"";
      // Caso a classe inversa esteja vazia: "[^]"
      if(lenF==3 && format[1]=='^') throw "Formato Vazio: \"[^]\"";
      // Caso a classe não termine com ']': "[abc"
      if(format[lenF-1] != ']') throw "Formato Inválido: \"[abc\"";
      // Caso o formato não comece com '[': "abc]"
      if(format[0]!='[') throw "Formato Inválido: \"abc]\"";
    
      // Verificando se é uma classe ou uma classe inversa,
      // Se for uma classe a posição de inicio é 2 se for inversa é 3.
      int j = format[1]!='^'?2:3;
      
      // Continuando com os testes:
      
      // Se o formato está no formato: "[abc]edf]"
      if(j+1<lenF)
        for( ; format[j+1]; j++)
          if(format[j] == ']') throw "Formato Inválido: \"[abc]edf]\"";
      // Note que no for acima note que o formato: []abc] ou [^]abc] é aceito.
      // Nesse caso não há erro pois o ']' será tratado como um caractere.
      
      
      // Caso o usuário escreva algo ambiguo no formato como: "[a--b]"
      j = format[1]!='^'?3:4;
      if(j+2<lenF)
        for( ; format[j+2]; j++)
          if(format[j-1]=='-' && format[j] =='-')
            throw "Formato Inválido: \"[a--b]\"";
      // Note no for acima que não há problema em escrever: "[--b]" ou "[a--]"
      
      // Caso o usuário escreva: "[a-b-c]" ou algo semelhante:
      j = format[1]!='^'?4:5;
      if(j+2<lenF)
        for( ; format[j+2]; j++)
          if(format[j-2]=='-' && format[j] =='-')
            throw "Formato Inválido: \"[a-b-c]\"";
    }
    
    /* * * * * * * Fim dos Testes de Validade * * * * * * */
  }
  
  pMatch::charClass::charClass(const char* format) :
    charClass(string(format)) {}
  
// Define usado para iterar por uma classe de caracteres:
// [abc] por exemplo.
#define forClass(c,i) \
  for(i=1; c[i]!=']' || i==1; i++)

// Define usado para iterar sobre uma classe inversa de caracteres:
// [^abc]
#define forNotClass(c,i) \
  for(i=2; c[i]!=']' || i==2; i++)

// /*
bool pMatch::charClass::match(string input, int pos)
{ return match(input[pos]); }
bool pMatch::charClass::match(char input)
  {
    charClass format = *this;
    
    // Length do formato:
    int lenF = format.length();
    
    // * * * * * * * Inicio da Execução Caso "a", "b" ou "c" * * * * * * *
    
    if(format[0] != '[' || lenF == 1)
      return input==format[0];
    
    // * * * * * * * Inicio da Execução Caso formato = "[abc]" * * * * * * *
    
    else if(format[1]!='^')
    // Quando o formato for do tipo "[abc]"
    {
      int j;
      // Iterando sobre a string format
      forClass(format, j)
      {
        // Caso o format seja do tipo: "[a-z]"
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
        // Caso o formato seja do tipo simples: "[abc]"
        else if( input == format[j] )
          return true;
      }
      return false;
    }
    
    // * * * * * * * Inicio da Execução Caso formato = "[^abc]" * * * * * * *
    
    else
    {
      int j;
      // Iterando sobre a string format
      forNotClass(format, j)
      {
        // Caso o format seja do tipo: "[^a-z]"
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
        
        // Caso o format seja do tipo: "[^abc]"
        if( input == format[j])
          return false;
      }
      
      return true;
    }
    
    // * * * * * * * Fim da Execução da Função * * * * * * *
    
    // Se o fluxo de execução chegar aqui há um erro no código.
    throw "Erro no codigo! Na funcao pMatch::charClass::match()!";
  }
// */

/*
 *
 * Função FindChar
 * @desc - Busca em uma string `input` por um caractere 
 *         dentre um conjunto de caracteres possíveis.
 *         
 *         Os caracteres possíveis são descritos na string `format`
 * 
 * @param - input: string a ser pesquisada.
 *        - format: string que define os caracteres à serem procurados.
 *        - pos: variável passada por referencia, contém a posição
 *               de inicio da busca no inicio da execução,
 *               e contém a posição do caractere encontrado
 *               no momento em que a execução termina.
 *
 * @return - retorna o caractere encontrado.
 *
 * @notas - o sistema aceita classes de caracteres.
 *          As classes aceitas atualmente são:
 *          
 *            "[abc]", "[a-x]", "[x-a]", "[abc-]", "[]]", "[]abc]"
 *          
 *          Também se aceita todas as inversas das classes acima:
 *
 *            "[^abc], "[^a-x]", "[^x-a]", "[^abc-]", "[^]]", "[^]abc]"
 *
 *          Por fim aceita-se também caracteres específicos:
 *
 *            "a", "b", "c" ... etc ...
 *
 *          O significado semantico de cada notação é o padrão para ERs.
 *          (foi baseado na semantica do LEX, que acredito ser padrão)
 *  
 */
char pMatch::charClass::find(string input, int& pos)
{
  charClass format = *this;
  
  int len = input.length();
  int i;
  
  if(pos<0 || pos>len)
    throw "Em pMatch::charClass::find() - Param 'pos' inválido";
  
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
 * @nome - charClass::getClass
 *
 * @desc - essa função constroi um charClass a partir de uma
 *         string formada por uma sequencia de strings 
 *         em formatos compatíveis com um charClass
 *
 *         Ao fim da execução o inteiro pos aponta para a posição
 *         de inicio da próxima busca nessa mesma string.
 *
 *         Assim é possível fazer um parse em uma string grande
 *         transformando-a em um array de charClasse`s.
 *
 */
// /*
  pMatch::charClass pMatch::charClass::getClass(string format, int& pos)
  {
    int i = pos;
    
    // Tratamento dos parâmetros:
    if(pos<0 || (unsigned)pos > format.length())
      throw "Variável pos inválida! pMatch::charClass::getClass";
    
    if((unsigned)pos == format.length())
      throw "String Vazia! pMatch::charClass::getClass";
    
    // Caso haja um caractere de escape.
    if(format[i]=='\\')
    {
      // A resposta é o próximo caractere da string:
      charClass resp(format.substr(i+1,1));
      
      // O ponteiro pos aponta para o primeiro caractere após
      // o caractere retornado.
      pos+=2;
      
      return resp;
    }
    // Caso seja um caractere simples e não uma classe de caracteres:
    else if(format[i]!='[')
    {
      // retorne esse caractere.
      charClass resp(format.substr(i,1));
      // Aponte o ponteiro para a posição seguinte.
      pos++;
      return resp;
    }
    
    // Se chegou aqui trata-se de um formato do tipo: "[abc]" ou "[^abc]"
    
    // Agora vamos buscar o caractere de fechamento: ']'
    i+=2;
    
    // Note que i+=2 pula o primeiro caractere da classe.
    // Isso ocorre para permitir a classe "[]abc]" ou "[]]"
    
    // E para permitir as classes: "[^]abc]" e "[^]]":
    if(format[i-1]=='^') i++;
    
    if( (unsigned)i < format.length() )
    
      // Para cada caractere a partir da posição atu[abc]al.
      for(; format[i]; i++)
      {
        // Se ele for o de fechamento de couchete retorne.
        if(format[i]==']')
        {
          // resp = classe: "[...]"
          charClass resp(format.substr(pos, i-pos+1));
          // pos é atualizado para a próxima posição:
          pos = i+1;
          
          return resp;
        }
      }
      // Note que por causa do i+=2 eu pulei a primeira posição.
      // Assim garanto que eu detectarei a classe []abc]
      // E não detectarei a classe [] (essa classe não existe)
    
    // Caso o formato abra um couchete mas não o feche
    // Ele caí em caso de erro:
    throw "Formato abre couchete mas não o fecha! pMatch::getClass";
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
   * @desc - Constroi uma lista formada por classe de caracteres.
   *         Note que é possível uma lista vazia.
   *         
   *         A interpretação format.length() para lista vazia
   *         é a palavra <lambda>
   *         ou seja, a palavra vazia.
   *         E ela é casada com qualquer string.
   *         
   */
  pMatch::strClass::strClass(string str)
  {
    int pos=0;
    
    // Tratando um erro possível:
    if(str[str.length()-1] == '\\')
      str.pop_back();
    
    while(str[pos])
      this->push_back( pMatch::charClass::getClass(str, pos) );
  }
  
  pMatch::strClass::strClass(const char* str) :
    strClass(string(str)) {}
  
  /*
   * @nome - strClass::match()
   * 
   * @desc - recebe uma string e compara os primeiros caracteres
   *         dessa string com seu formato.
   *         retornando true se forem iguais.
   *
   *         note que a strClass("").match(str,pos) retorna true sempre.
   *
   */
  bool pMatch::strClass::match(string input, int pos)
  {
    // Limpa a lista antiga:
    this->match_word.clear();
    
    int len = this->size();
    strClass::iterator it = this->begin();
    
    for(int i=0; i<len; i++, it++)
    {
      if(!it->match(input[pos+i]))
        return false;
    }
    
    // Adiciona o novo item a lista e retorna:
    this->match_word.push_back(tWord(input.substr(pos,len), pos));
    return true;
  }

  /*
   * @nome - strClass::find()
   * 
   * @desc - recebe uma string input, e caminha por ela até encontrar
   *         o primeiro match entre o seu formato e uma substring de input.
   *         
   *         Note que aceita-se a string vazia.        
   * 
   */
  pMatch::tWord pMatch::strClass::find(string input, int pos)
  {
    int lenF = this->size();
    
    // Se o formato for de string vazia:
    if(lenF == 0)
      // Aceite e retorne outra string vazia:
      return tWord("", pos);
    
    strClass::iterator it=this->begin();
    
    int i=pos;
    
    // Encontra o primeiro caractere do input compatível com o formato:
    (*it).find(input, i); 
    
    while(input[i])
    {
      
      if(this->match(input, i))
      {
        pos=i;
        return tWord(input.substr(i,lenF),i);
      }
      else i++;
      
      // Encontra o primeiro caractere do formato:
      (*it).find(input, i); 
    }
    
    // Caso não encontre:
    return tWord("", i);
  }
  
  pMatch::strClass pMatch::strClass::getClass(string input, int& pos)
  {
    int start = pos;
    char c;
    
    if(pos<0 || (unsigned)pos > input.length())
      throw "variável pos com valor inválido! pMatch::strClass::getClass";
     
    while(1)
    {
      // Pesquise pela string em busca de um abre parentesis:
      c = charClass("(").find(input, pos);
      
      // Se encontrou um parentesis, mas ele tinha uma barra antes:
      if(c=='(' && input[pos-1] == '\\')
      {
        // ignore-o
        pos++;
        continue;
      }
      else 
      {
        // Caso contrário retorne a string encontrada:
        return strClass(input.substr(start, pos-start));
      }
    }
  }
  
  // Gera uma string representando o conteúdo do objeto:
  string pMatch::strClass::str()
  {
    strClass::iterator it;
    string resp("");
    
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
  
