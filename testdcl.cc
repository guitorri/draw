/*
   Code adapted from:
   Dynamic Class Loading for C++ on Linux, May 01, 2000  By James Norton
   http://www.linuxjournal.com/article/3687
*/

#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <string>

#ifdef __MINGW32__
  #include <windows.h>
#else
  #include <dlfcn.h>
#endif

#include <stdio.h>
#include <unistd.h>
#include "shape.hh"

#include <cstdlib> //for exit
#include <string.h> //for strpbrk

class proxy;

// size of buffer for reading in directory entries
static unsigned int BUF_SIZE = 1024;

// our global factory for making shapes
std::map<std::string, maker_t *, std::less<std::string> > factory;

int main(int argc, char **argv){

  using namespace std;
  FILE *dl;   // handle to read directory
#ifdef __APPLE__
  char *command_str = (char *)"ls *.dylib";  // command string to get dynamic lib names
#endif
#ifdef __linux__
  char *command_str = (char *)"ls *.so";  // command string to get dynamic lib names
#endif
#ifdef __MINGW32__
  char *command_str = (char *)"dir /B *.dll";  // bare formant (no header or other info)
#endif
  char in_buf[BUF_SIZE]; // input buffer for lib names


#if __MINGW32__
  list<HINSTANCE> dl_list; // list to hold handles for dynamic libs
  list<HINSTANCE>::iterator itr;
#else
  list<void *> dl_list; // list to hold handles for dynamic libs
  list<void *>::iterator itr;
#endif

  vector<string> shape_names;  // vector of shape types used to build menu
  list<shape *> shape_list;  // list of shape objects we create
  list<shape *>::iterator sitr;
  map<string, maker_t *, less<string> >::iterator fitr;
  // get the names of all the dynamic libs (.so/.dylib/.dll files) in the current dir
  dl = popen(command_str, "r");
  if(!dl){
    perror("popen");
    exit(-1);
  }
  //void *dlib;
  char name[1024];

  while(fgets(in_buf, BUF_SIZE, dl)){
    // trim off the whitespace
    char *ws = strpbrk(in_buf, " \t\n");
    if(ws) *ws = '\0';
    // append ./ to the front of the lib name
    sprintf(name, "./%s", in_buf);

    // which lib is going to be loaded
    fprintf( stdout, "try loading %s\n", name  );


#if __MINGW32__
// Load the DLL
    HINSTANCE dlib = ::LoadLibrary(TEXT(name));
    if (!dlib) {
        cerr << "Unable to load DLL!\n";
        return 1;
    }
#else //Linux and OSX
    // for some reason the RTLD_NOW alones makes dlopen
    // stick with the name (content) of the first loaded library
    void* dlib = dlopen(name, RTLD_NOW|RTLD_LOCAL);
    if(dlib == NULL){
      cerr << dlerror() << endl;
      exit(-1);
    }
#endif


    // add the handle to our list
    dl_list.insert(dl_list.end(), dlib);

  }

  std::cout << "dl_list contains handles:";
  for (itr=dl_list.begin(); itr!=dl_list.end(); ++itr)
    std::cout << ' ' << *itr;
  std::cout << '\n';

  // report on factory
  std::cout << "factory.size() is " << factory.size() << '\n';
  std::cout << "factory contains:";
  for (fitr=factory.begin(); fitr!=factory.end(); ++fitr)
    std::cout << ' ' << fitr->first;
  std::cout << '\n';

  int i = 0;
  // create an array of the shape names
  for(fitr=factory.begin(); fitr!=factory.end();
      fitr++){
    shape_names.insert(shape_names.end(),
        fitr->first);
    i++;
  }

  int choice;
  // create a menu of possible shapes to create and let the user make some
  while(1){
    i = 1;
    for(fitr=factory.begin();
        fitr!=factory.end(); fitr++){
      cout << i << " - Create " << fitr->first
        << endl;
      i++;
    }
    cout << i << " - Draw created shapes\n";
    i++;
    cout << i << " - Exit\n";
    cout << "> ";
    cin >> choice;
    if(choice == i){
      // destroy any shapes we created
      for(sitr=shape_list.begin();
          sitr!=shape_list.end();sitr++){
        delete *sitr;
      }
      // close all the dynamic libs we opened
      for(itr=dl_list.begin(); itr!=dl_list.end(); itr++){
        //dlclose(*itr);
      }
      exit(1);
    }
    if(choice == i - 1){
      // draw the shapes
      for(sitr=shape_list.begin();
          sitr!=shape_list.end();sitr++){
        (*sitr)->draw();
      }
    }
    if(choice > 0 && choice < i - 1){
      // add the appropriate shape to the shape list
      shape_list.insert(shape_list.end(),
          factory[shape_names[choice-1]]());
    }
  }
}

