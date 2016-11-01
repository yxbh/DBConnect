# #!/bin/bash
# set -ev

cd src

qmake DBConnect.pro "CONFIG+=debug"
make

make clean
qmake DBConnect.pro
make
