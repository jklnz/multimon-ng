multimon-ng Windows Build


This fork is used for providing an updated Windows build.

You must add this two flags to multimon-ng.pro for it to build a portable executable on Windows, they are:
```
QMAKE_LFLAGS += -static-libgcc
QMAKE_LFLAGS += -static-libstdc++
```
To build multimon-ng on Windows using qmake/mingw32/qt:
```
mkdir build
cd build
qmake ../multimon-ng.pro
mingw32-make
```



