#!/bin/bash

# fpp-plugin-controller install script

BASEDIR=$(dirname $0)
cd $BASEDIR
cd ..

#sudo apt-get -y update
#sudo apt-get -y install libasio-dev --no-install-recommends

make
