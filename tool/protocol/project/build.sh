#!/bin/bash

rm -rf ../../../gen/protocol
mkdir -p ../../../gen/protocol/script
protocols=`ls protocols`
../script/gen.lua ./protocols $protocols
