#include <string>
#include <map>
#include <vector>

#include "padrao.hpp"

#ifndef TALKIN_VARS
#define TALKIN_VARS


namespace vars
{
  class cObject
  {
public:
    bool bBuild=false;
    void build(pMatch::tInterpretacao tInt);
    
    std::string valor;

    std::map<std::string,cObject> subvars;
    std::vector<cObject> array;

    // Denota a profundidade da variável em relação a raiz:
    int deep=0;
    bool defined=true;
    
    // A variável abaixo é usada para denotar
    // uma variável inválida.
    // A construtora abaixo só deve ser usada para as variáveis abaixo.
    cObject(bool undefined){if(undefined) this->defined=false;}
    static cObject undefined;
    static cObject null;

    // Construtora utilizada nas recursões para construir
    // uma arvore de variáveis com base em pMatch::cObject:
    cObject(pMatch::cVar var, int deep);

    public:
    // constroi um objeto vazio
    cObject();
    ~cObject();

    // Constroi um objeto do ambiente com base em uma variável do pMatch.
    cObject(pMatch::cVar var);
    cObject(pMatch::tInterpretacao);
    cObject(const char* valor);
    cObject(std::string valor);

    public:
    // Getters & setters:
    std::string getValor(){return this->valor;}
    void setValor(std::string str){this->valor = str;}
    bool is_defined(){return defined;}

    //
    // @name - child
    //
    // @desc - Recebe um endereço do tipo: a.b[10].c...
    //         e retorna se existir a referência
    //         do objeto correspondente.
    //
    // @params - endereco: hierarquia do objeto filho em relação a este.
    //         - create: booleano, se `true` novas variaveis serão alocadas
    //           quando o endereço referenciar variáveis inexistentes.
    //
    // @return - Em caso de sucesso retorna o objeto desejado ou um recém alocado.
    //         - Em caso de fracasso retorna o objeto cObject::undefined.
    //
    // (tested)
    cObject& child(std::string endereco, bool create=false);

    // @name - operator[]
    // Funciona como a função child com create=false
    // mas retorna um ponteiro para o objeto encontrado.
    cObject* operator[](std::string& endereco);

    // Not tested:
    bool operator==(std::string& str);
    bool operator==(cObject& obj);
    bool operator==(bool& b);
    operator bool();

    // Tested:
    std::string str();

    private:
    // Tested:
    cObject& rec_child(std::string& endereco, int pos, bool create=false);
  };
}

#endif
