mkdir -p bin
mkdir -p log

read -p "ATTENTION: before run build.sh, you need install valgrind, mysql, openssl" X
DIR=`pwd`

cd dependence
./install.sh || exit

cd $DIR

cd build
make

cd $DIR
