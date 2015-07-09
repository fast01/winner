#!/bin/bash

# func
function die(){
	echo $!
	exit 1
}

# prepare var
work_dir=`pwd`
echo "work dir is $work_dir"

mkdir -p ../include || die "fail to create inc dir"
cd ../include || die "fail to cd inc dir"
inc_ins_dir=`pwd`
echo "inc install dir is $inc_ins_dir"

cd $work_dir
mkdir -p ../lib || die "fail to create lib dir"
cd ../lib || die "fail to cd lib dir"
lib_ins_dir=`pwd`
echo "lib install dir is $lib_ins_dir"

# install lua
lua_incs="lua.h luaconf.h lualib.h lauxlib.h lua.hpp"
echo "----- install lua -----"
cd $work_dir || die "fail to cd $work_dir"
cd lua-5.3.0 || die "fail to cd lua-5.3.0"
echo "making lua"
make linux || die "fail to make lua"
echo "copying files"
cd src || die "fail to copy files"
inc_dir=$inc_ins_dir/lua
lib_dir=$lib_ins_dir/lua
mkdir -p $inc_dir || die "fail to mkdir inc dir"
mkdir -p $lib_dir || die "fail to mkdir lib dir"
rm -f $inc_dir/* || die "fail to clean inc dir"
rm -f $lib_dir/* || die "fail to clean lib dir"
cp -t $inc_dir $lua_incs || "fail to copy inc"
cp -t $lib_dir liblua.a || "fail to copy lib"

# install tinyxml
tinyxml_incs="tinyxml.h tinystr.h"
echo "----- install tinyxml -----"
cd $work_dir || die "fail to cd $work_dir"
cd tinyxml || die "fail to cd tinyxml"
echo "making tinyxml"
make || die "fail to make tinyxml"
ar -rv libtinyxml.a tinystr.o tinyxml.o tinyxmlparser.o tinyxmlerror.o || "fail to ar tinyxml.a"
echo "copying files"
inc_dir=$inc_ins_dir/tinyxml
lib_dir=$lib_ins_dir/tinyxml
mkdir -p $inc_dir || die "fail to mkdir inc dir"
mkdir -p $lib_dir || die "fail to mkdir lib dir"
rm -f $inc_dir/* || die "fail to clean inc dir"
rm -f $lib_dir/* || die "fail to clean lib dir"
cp -t $inc_dir $tinyxml_incs || "fail to copy inc"
cp -t $lib_dir libtinyxml.a || "fail to copy lib"

# install cJSON
cjson_incs="cJSON.h"
echo "----- install cJSON -----"
cd $work_dir || die "fail to cd $work_dir"
cd cJSONFiles/cJSON || die "fail to cd cJSONFiles/cJSON"
echo "making cJSON"
g++ -c -o cJSON.o cJSON.c
ar -rv libcjson.a cJSON.o || "fail to ar cJSON.a"
echo "copying files"
inc_dir=$inc_ins_dir/cJSON
lib_dir=$lib_ins_dir/cJSON
mkdir -p $inc_dir || die "fail to mkdir inc dir"
mkdir -p $lib_dir || die "fail to mkdir lib dir"
rm -f $inc_dir/* || die "fail to clean inc dir"
rm -f $lib_dir/* || die "fail to clean lib dir"
cp -t $inc_dir $cjson_incs || "fail to copy inc"
cp -t $lib_dir libcjson.a || "fail to copy lib"

# echo success
echo "----- all success -----"
