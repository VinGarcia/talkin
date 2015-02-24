#include <iostream>
#include <fstream>
#include "ambiente.hpp"

using namespace std;
using namespace ambiente;

// /*
int main(int argc, char* argv[])
{
  try{
    // Construir:
    inicializa_banco();
  
    // Carregar drivers:
    drivers["stdout"] = new driver_stdout();
    drivers["talkin"] = new driver_talkin();
    
    // Carrego as instruções 
    std::ifstream file("vocabulario.talk");
    if(file.is_open()) readVocab(file);
    else cout << "Vocabulário não encontrado!\n";
    
    // Chamar modulo de interação com o usuário:
    interface();
    
  } catch(const char* c) { cout << c << endl; }
  
  cout << "bye!" << endl;
  
  return 0;
}
// */
