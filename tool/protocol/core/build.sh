#!/bin/bash

rm -rf ../../../core/gen/protocol
mkdir -p ../../../core/gen/protocol/script
protocols=`ls protocols`
../script/gen.lua ./protocols $protocols
