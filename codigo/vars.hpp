#include <string>
#include <map>
#include <vector>

#include "padrao.hpp"
#include "exp-parser/shunting-yard.h"

#ifndef TALKIN_VARS
#define TALKIN_VARS


namespace vars {

  class cObject : public TokenMap_t {

  public:
    void build(pMatch::tInterpretacao tInt);
    
    void checkAddr(const std::string& str);

  public:
    // constroi um objeto vazio
    cObject();
    ~cObject();

    // Constroi um objeto do ambiente com base em uma variável do pMatch.
    cObject(pMatch::cVar var);
    cObject(pMatch::tInterpretacao tInt);
    cObject(const char* valor);
    cObject(const std::string& valor);

    // Tested:
    std::string str();
  };
}

#endif
