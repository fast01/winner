#!/bin/bash

rm -rf ../../../core/gen/cpplua
mkdir -p ../../../core/gen/cpplua
prototypes=`ls prototypes`
../script/gen.lua ./prototypes $prototypes
