
# Dynamic Loader

Code adapted from:

Dynamic Class Loading for C++ on Linux, May 01, 2000  By James Norton.

   <http://www.linuxjournal.com/article/3687>

The comments on the article are also worth reading.

# Tested with:

* Mac OSX 10.8.5, Apple LLVM version 4.2 (clang-425.0.28)
* Debian 6.0.7, g++ 4.5.5
* Windows XP, MingGW 4.8
  * NOT WORKING!. It complains about unresolved `factory` symbol on the libraries. No compiler/linker option seems to let the external symbol unresolved. Which is defined on the main application, not on the libraries.

# Compile and run:

Compile:

```
$ make all
```

Run:

```
~/git/draw $ ./testdcl
try loading ./libcircle.dylib
  --> hi from circle
try loading ./libsquare.dylib
  --> hi from square
dl_list contains handles: 0x7f8513c039d0 0x7f8513c03e30
factory.size() is 2
factory contains: circle square
1 - Create circle
2 - Create square
3 - Draw created shapes
4 - Exit
> 1
1 - Create circle
2 - Create square
3 - Draw created shapes
4 - Exit
> 2
1 - Create circle
2 - Create square
3 - Draw created shapes
4 - Exit
> 3

      ****
    *      *
   *        *
   *        *
   *        *
    *      *
      ****


    *********
    *       *
    *       *
    *       *
    *       *
    *********

1 - Create circle
2 - Create square
3 - Draw created shapes
4 - Exit
> 4
~/git/draw $
```
