#!/bin/bash
## func
function print_help(){
	echo "example:"
	echo "    ./gen.sh -c ClassName -d HEADER_ID outdir"
	echo "options:"
	echo "    -f or --file: special the file name"
	echo "    -d or --header-id : special the header id"
	echo "    -i or --inc: special the included file name"
	echo "    -n or --namespace: special the namespace name"
	echo "    -s or --super : special the super class name"
	echo "    -c or --class : special the class name"
	echo "    -h or --help : used to see help information"
	exit
}
## prepare
INC=core.h
NAMESPACE=core
SUPER=Object
ENTER_DIR=`pwd`
DIR=$ENTER_DIR

## parse arg
while [ $# -gt 0 ]; do
	if [ $# -ge 2 ]; then
		if [ $1 == '-f' -o $1 == '--file' ]; then
			FILE=$2
		elif [ $1 == '-d' -o $1 == '--header-id' ]; then
			HEADER_ID=$2
		elif [ $1 == '-i' -o $1 == '--inc' ]; then
			INC=$2
		elif [ $1 == '-n' -o $1 == '--namespace' ]; then
			NAMESPACE=$2
		elif [ $1 == '-s' -o $1 == '--super' ]; then
			SUPER=$2
		elif [ $1 == '-c' -o $1 == '--class' ]; then
			CLASS=$2
		elif [ $1 == '-h' -o $1 == '--help' ]; then
			print_help
		else
			echo "unknown option $1"
			echo "try with option -h or --help for more information."
			exit
		fi
		shift 2
	else
		if [ $1 == '-h' -o $1 == '--help' ]; then
			print_help
		else
			DIR=$1
		fi
		shift
	fi
done

## preprocess
if [ "$CLASS" == '' ]; then
	echo "missing class name"
	exit
fi
if [ "$FILE" == '' ]; then
	FILE=$CLASS
fi
if [ "$HEADER_ID" == '' ]; then
	echo "missing header id"
	exit
fi
echo "FILE      : $FILE"
echo "HEADER_ID : $HEADER_ID"
echo "INC       : $INC"
echo "NAMESPACE : $NAMESPACE"
echo "SUPER     : $SUPER"
echo "CLASS     : $CLASS"
echo "DIR       : $DIR"
mkdir -p tmp

## generate header
sed "s/\${HEADER_ID}/${HEADER_ID}/g" class.h.tmpl > tmp/a
sed "s/\${NAMESPACE}/${NAMESPACE}/g" tmp/a > tmp/b
sed "s/\${SUPER}/${SUPER}/g" tmp/b > tmp/c
sed "s/\${CLASS}/${CLASS}/g" tmp/c > tmp/$FILE.h

## generate cpp
sed "s/\${INC}/${INC}/g" class.cpp.tmpl > tmp/a
sed "s/\${NAMESPACE}/${NAMESPACE}/g" tmp/a > tmp/b
sed "s/\${SUPER}/${SUPER}/g" tmp/b > tmp/c
sed "s/\${CLASS}/${CLASS}/g" tmp/c > tmp/$FILE.cpp

## move
cp tmp/$FILE.h $DIR && cp tmp/$FILE.cpp $DIR && echo "generate success"
rm -fr tmp
