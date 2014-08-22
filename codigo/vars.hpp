#include <string>
#include <map>
#include <vector>

#include "padrao.hpp"

#ifndef TALKIN_VARS
#define TALKIN_VARS


namespace vars
{
  class cVar
  {
public:
    bool bBuild=false;
    void build(pMatch::tInterpretacao tInt);
    
    std::string nome;
    std::string valor;

    std::map<std::string,cVar> subvars;
    std::vector<cVar> array;

    // Aponta para a variável raiz:
    cVar* grand_father=NULL;
    // Denota a profundidade da variável em relação a raiz:
    int deep=0;
    bool var_defined=true;
    
    // A variável abaixo é usada para denotar
    // uma variável inválida.
    // A construtora abaixo só deve ser usada para essa variável.
    cVar(bool undefined){if(undefined) this->var_defined=false;}
    static cVar undefined;
    // A string abaixo guarda o endereço acessado pelo último
    // acesso ao operador [] que retornou 'undefined'.
    // O objetivo é permitir que o usuário faça uma atribuição
    // sobre undefined no endereço abaixo.
    std::string undef_addr;
    // Pai da variável almejada pelo operador []
    // (valor retornado pela getNearestVar())
    cVar* undef_father;
    // NOTA EXPLICATIVA:
    // undefined só é retornado pela função operator[] e/ou getVar.
    // Em ambos os casos o endereço que tentou-se acessar é salvo
    // na variável acima.
    // Caso tente-se atribuir na forma:
    //   
    //   var["endereco.nao.definido"] = cVar(nome,valor);
    //
    // cria-se o endereço "endereco.nao.definido" em uma nova cVar,
    // mantendo-se a variável 'undefined' inalterada.
    // Caso a atribuição seja do tipo:
    //
    //   cVar(nome,valor) = var["endereco.nao.definido"]
    //
    // Cria-se uma cVar() vazia e só então realiza-se a atribuição.
    // Dessa forma undefined fica sempre inalterada, e o usuário
    // nunca é capaz de acessa-la diretamente, pois ela é sempre protegida.
    //
    // Note que em ambos os casos acima utiliza-se a sobrecarga
    // do operador '='.

    // Construtora utilizada nas recursões para construir
    // uma arvore de variáveis com base em pMatch::cVar:
    cVar(pMatch::cVar var, cVar* grand_father, int deep);

    public:
    // Constroi uma variável do ambiente com base em uma variável do pMatch.
    cVar();
    ~cVar();

    //cVar(std::string nome, std::string valor);
    cVar(pMatch::cVar var);
    cVar(std::string nome, pMatch::tInterpretacao);
    cVar(std::string nome, std::string valor);

    public:
    // Getters & setters:
    std::string getNome(){return this->nome;}
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
    cVar* getVar(std::string endereco);
    bool exists(std::string endereco){return (bool)getVar(endereco);}


    void addChild(std::string nome, std::string valor);
    void addChild(cVar child);
    cVar& addVar(std::string endereco_var, std::string valor);

    // @name - operator[]
    // Funciona como a função getVar.
    cVar* operator[](std::string endereco);

    cVar& setValor(std::string valor);
    std::string str();

    private:
    cVar& addVar(std::string endereco_var, std::string valor, cVar* nearestVar);
    std::pair<bool,cVar&> getNearestVar(std::string endereco);
    std::pair<bool,cVar&> rec_getNearestVar(std::string& endereco, int pos);
  };
}

#endif
