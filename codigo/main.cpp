#include <iostream>
#include "padrao.hpp"

using namespace std;
using namespace pMatch;

int main(int argc, char* argv[])
{
  string entrada;
  
  while(1)
  {
    cin >> entrada;
    if(entrada=="exit") return 0;
    cout << arrayClass(entrada).str() << endl;
  }
}
