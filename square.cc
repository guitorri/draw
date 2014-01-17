#include <iostream>
#include "square.hh"
void square::draw(){
  using namespace std;
  // simple ascii square
  cout << "\n";
  cout << "    *********\n";
  cout << "    *       *\n";
  cout << "    *       *\n";
  cout << "    *       *\n";
  cout << "    *       *\n";
  cout << "    *********\n";
  cout << "\n";
}


extern "C" { EXPORT
  shape *maker(){
    return new square;
  }
  class proxy {
    public:
      proxy(){
        // register the maker with the factory
        factory["square"] = maker;
        std::cout << "  --> hi from square\n";
      }
  };
  // our one instance of the proxy
  proxy p;
}
