#ifdef __MINGW32__
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif


#ifndef __SHAPE_H
#define __SHAPE_H
#include <map>
#include <string>
// base class for all shapes
class shape {
  public:
    virtual void draw()=0;
};
// typedef to make it easier to set up our factory
typedef shape *maker_t();

// our global factory
extern "C" {
 extern EXPORT std::map<std::string, maker_t *, std::less<std::string> > factory;
}
#endif // __SHAPE_H

