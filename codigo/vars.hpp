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

    // Aponta para a variável raiz:
    cObject* grand_father=NULL;
    // Denota a profundidade da variável em relação a raiz:
    int deep=0;
    bool var_defined=true;
    
    // A variável abaixo é usada para denotar
    // uma variável inválida.
    // A construtora abaixo só deve ser usada para as variáveis abaixo.
    cObject(bool undefined){if(undefined) this->var_defined=false;}
    static cObject undefined;
    static cObject null;
    // A string abaixo guarda o endereço acessado pelo último
    // acesso ao operador [] que retornou 'undefined'.
    // O objetivo é permitir que o usuário faça uma atribuição
    // sobre undefined no endereço abaixo.
    std::string undef_addr;
    // Pai da variável almejada pelo operador []
    // (valor retornado pela getNearestVar())
    cObject* undef_father;
    // NOTA EXPLICATIVA:
    // undefined só é retornado pela função operator[] e/ou getVar.
    // Em ambos os casos o endereço que tentou-se acessar é salvo
    // na variável acima.
    // Caso tente-se atribuir na forma:
    //   
    //   var["endereco.nao.definido"] = cObject(valor);
    //
    // cria-se o endereço "endereco.nao.definido" em uma nova cObject,
    // mantendo-se a variável 'undefined' inalterada.
    // Caso a atribuição seja do tipo:
    //
    //   cObject(valor) = var["endereco.nao.definido"]
    //
    // Cria-se uma cObject() vazia e só então realiza-se a atribuição.
    // Dessa forma undefined fica sempre inalterada, e o usuário
    // nunca é capaz de acessa-la diretamente, pois ela é sempre protegida.
    //
    // Note que em ambos os casos acima utiliza-se a sobrecarga
    // do operador '='.

    // Construtora utilizada nas recursões para construir
    // uma arvore de variáveis com base em pMatch::cObject:
    cObject(pMatch::cVar var, cObject* grand_father, int deep);

    public:
    // Constroi uma variável do ambiente com base em uma variável do pMatch.
    cObject();
    ~cObject();

    cObject(pMatch::cVar var);
    cObject(pMatch::tInterpretacao);
    cObject(const char* valor);
    cObject(std::string valor);

    public:
    // Getters & setters:
    std::string getValor(){return this->valor;}
    bool is_defined(){return var_defined;}

    //
    // @name - getVar
    //
    // @desc - Recebe um endereço do tipo: a.b.c...
    //         e retorna se existir o valor da variável
    //         correspondente entre suas subvars.
    //
    // @return - Em caso de sucesso .first = true, .second = valor em string
    //         - Em caso de fracasso .first = false
    //
    cObject* getVar(std::string endereco);
    bool exists(std::string endereco){return (bool)getVar(endereco);}

    void addChild(std::string nome, const char* valor);
    void addChild(std::string nome, std::string valor);
    void addChild(std::string nome, cObject child);
    cObject& addVar(std::string endereco_var, std::string valor);

    // @name - operator[]
    // Funciona como a função getVar.
    cObject* operator[](std::string endereco);

    cObject& setValor(std::string valor);
    std::string str();

    cObject& child(std::string endereco, bool create=false);

    private:
    cObject& rec_child(std::string& endereco, int pos, bool create=false);

    cObject& addVar(std::string endereco_var, std::string valor, cObject* nearestVar);
    std::pair<bool,cObject&> getNearestVar(std::string endereco, bool create=false);
    std::pair<bool,cObject&> rec_getNearestVar(std::string& endereco, int pos, bool create=false);
  };
}

#endif
