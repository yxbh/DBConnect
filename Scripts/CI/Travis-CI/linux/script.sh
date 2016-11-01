#!/bin/bash
set -ev

pwd
cd src

qmake DBConnect.pro "CONFIG+=debug"
make

make clean
qmake DBConnect.pro
make
