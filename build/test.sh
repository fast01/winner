# export HEAPCHECK=strict
# ./project
# CPUPROFILE="./project.prof" ./project
# HEAPPROFILE="./project.prof" ./project
# pprof --pdf ./project ./project.prof > project.pdf

# valgrind --tool=helgrind ./project
# valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./project ../data0
#valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./project ../examples/http/data
valgrind --tool=memcheck --leak-check=full ./project ../examples/http/data

# echo "please modify this file by yourself"
