#include <iostream>
#include "circle.hh"
void circle::draw(){
  using namespace std;
  // simple ascii circle
  cout << "\n";
  cout << "      ****\n";
  cout << "    *      *\n";
  cout << "   *        *\n";
  cout << "   *        *\n";
  cout << "   *        *\n";
  cout << "    *      *\n";
  cout << "      ****\n";
  cout << "\n";
}

extern "C" { EXPORT
  shape *maker(){
    return new circle;
  }
  class proxy {
    public:
      proxy(){
        // register the maker with the factory
        factory["circle"] = maker;
        std::cout << "  --> hi from circle\n";
      }
  };
  // our one instance of the proxy
  proxy p;
}

