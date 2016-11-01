# go to source directory.
pwd
cd src

# build DEBUG build.
qmake DBConnect.pro "CONFIG+=debug"
mingw32-make

# build RELEASE build.
qmake DBConnect.pro
mingw32-make