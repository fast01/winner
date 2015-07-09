#!/bin/bash

rm -rf ../../../gen/cpplua
mkdir -p ../../../gen/cpplua
prototypes=`ls prototypes`
../script/gen.lua ./prototypes $prototypes
