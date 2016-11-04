# go to source directory.
pwd
cd src

# build DEBUG build.
qmake DBConnect.pro "CONFIG+=debug"
IF ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

mingw32-make
IF ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

# build RELEASE build.
qmake DBConnect.pro
IF ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

mingw32-make
IF ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
