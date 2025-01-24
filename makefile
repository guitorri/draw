
# default
EXE = testdcl
LIBS = -ldl

ifeq ($(OS),Windows_NT)
  CC = g++
  DYN = dll
  LIBS =
  # need import library to resolve DLL extern symbol factory (defined on testdcl.cc)
  # http://stackoverflow.com/questions/17601949/building-a-shared-library-using-gcc-on-linux-and-mingw-on-windows
  LINKFLAGS = -Wl,--out-implib,libtestdcl.a
else
  UNAME_S = $(shell uname -s)

# Linux
  ifeq ($(UNAME_S),Linux)
  CC = g++
  CCFLAGS = -rdynamic -fPIC
  DYN = so
  endif

# OS X
  ifeq ($(UNAME_S),Darwin)
  CC = clang++
  DYN = dylib
  endif
endif

#.cc.o:
#	$(CC) -ggdb -c $<

%.o : %.cc
	$(CC) -c $(CCFLAGS) $< -o $@

default:
	make testdcl

$(EXE): testdcl.o
	$(CC) $(CCFLAGS) -o testdcl testdcl.o $(LIBS) $(LINKFLAGS)


# Linux libs targets
libcircle.so:  circle.o
	$(CC) -shared -Wl,-soname,libcircle.so -o libcircle.so circle.o

libsquare.so:  square.o
	$(CC) -shared -Wl,-soname,libsquare.so -o libsquare.so square.o


# OSX libs targets
libcircle.dylib:  circle.o
	$(CC) -dynamiclib -install_name libcircle.dylib \
  -o libcircle.dylib circle.o -undefined dynamic_lookup

libsquare.dylib:  square.o
	$(CC) -dynamiclib -install_name libsquare.dylib \
  -o libsquare.dylib square.o -undefined dynamic_lookup


# MINGW32 libs targets
libcircle.dll:  circle.o
	$(CC) -shared -o libcircle.dll circle.o -L . -ltestdcl


libsquare.dll :  square.o
	$(CC) -shared  -o libsquare.dll square.o -L . -ltestdcl

all: testdcl libcircle.$(DYN) libsquare.$(DYN)

libs: libcircle.$(DYN) libsquare.$(DYN)

clean:
	rm -f *.so *.o testdcl *.dylib *.dll *.exe *.a
