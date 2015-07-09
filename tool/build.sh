#!/bin/bash
ROOT=`pwd`

# cpplua
cd cpplua
./build.sh
cd $ROOT

# protocol
cd protocol
./build.sh
cd $ROOT
