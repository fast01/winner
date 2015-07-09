##### prepare compile option ####
liblua     := ../lib/lua/liblua.a
libtinyxml := ../lib/tinyxml/libtinyxml.a
libcjson   := ../lib/cJSON/libcjson.a
libmysql   := -L/usr/lib/mysql -lmysqlclient
CC       := g++
LDFLAGS  := -lstdc++ -rdynamic -ldl -lpthread -lm -lcrypto $(libmysql) $(liblua) $(libtinyxml) $(libcjson)
CXXFLAGS := -Wall -Werror -fno-strict-aliasing -g -c -rdynamic -std=c++0x -I/usr/include/mysql

default : project

#### prepare var ####
framework_dir   := ..
bindir    := $(framework_dir)/bin

#### prepare core ####
core_incs := $(wildcard $(framework_dir)/core/*.h) $(wildcard $(framework_dir)/core/*.hpp) $(wildcard $(framework_dir)/core/gen/data/*.h) $(wildcard $(framework_dir)/core/gen/protocol/*.h) $(wildcard $(framework_dir)/core/gen/cpplua/*.h)
core_srcs := $(wildcard $(framework_dir)/core/*.cpp) $(wildcard $(framework_dir)/core/gen/data/*.cpp) $(wildcard $(framework_dir)/core/gen/protocol/*.cpp) $(wildcard $(framework_dir)/core/gen/cpplua/*.cpp)
core_objs := $(patsubst %.cpp,%.o,$(core_srcs))
core_h    := $(framework_dir)/core/core.h
core_gch  := $(framework_dir)/core/core.h.gch

# precompile header
$(core_gch) : $(core_incs)
	$(CC) -o $(core_gch) -x c++-header -c $(core_h) $(CXXFLAGS)

$(core_objs) : %.o : %.cpp $(core_gch)
	$(CC) $(CXXFLAGS) -o $@ $<

#### prepare protocol ####
protocol_incs := $(core_incs) $(wildcard $(framework_dir)/gen/protocol/*.h)
protocol_srcs := $(wildcard $(framework_dir)/gen/protocol/*.cpp) 
protocol_objs := $(patsubst %.cpp,%.o,$(protocol_srcs))

$(protocol_objs) : %.o : %.cpp $(protocol_incs)
	$(CC) $(CXXFLAGS) -o $@ $<

#### prepare cpplua ####
cpplua_incs := $(core_incs) $(wildcard $(framework_dir)/gen/cpplua/*.h)
cpplua_srcs := $(wildcard $(framework_dir)/gen/cpplua/*.cpp) 
cpplua_objs := $(patsubst %.cpp,%.o,$(cpplua_srcs))

$(cpplua_objs) : %.o : %.cpp $(cpplua_incs)
	$(CC) $(CXXFLAGS) -o $@ $<

#### prepare test ####
test_incs := $(protocol_incs) $(cpplua_incs) $(wildcard $(framework_dir)/test/*.h)
test_srcs := $(wildcard $(framework_dir)/test/*.cpp) 
test_objs := $(patsubst %.cpp,%.o,$(test_srcs))
test_h    := $(framework_dir)/test/test.h
test_gch  := $(framework_dir)/test/test.h.gch

# precompile header
$(test_gch) : $(test_incs)
	$(CC) -o $(test_gch) -x c++-header -c $(test_h) $(CXXFLAGS)

$(test_objs) : %.o : %.cpp $(test_gch) 
	$(CC) $(CXXFLAGS) -o $@ $<

#### prepare service ####
service_incs := $(test_incs) $(wildcard $(framework_dir)/service/*/*.h) $(wildcard $(framework_dir)/service/*.h)
service_srcs := $(wildcard $(framework_dir)/service/*/*.cpp) $(wildcard $(framework_dir)/service/*.cpp)
service_objs := $(patsubst %.cpp,%.o,$(service_srcs))
service_h    := $(framework_dir)/service/service.h
service_gch  := $(framework_dir)/service/service.h.gch

# precompile header
$(service_gch) : $(service_incs)
	$(CC) -o $(service_gch) -x c++-header -c $(service_h) $(CXXFLAGS)

$(service_objs) : %.o : %.cpp $(service_gch)
	$(CC) $(CXXFLAGS) -o $@ $<

#### target project_debug ####
project_incs := $(service_incs) $(wildcard $(framework_dir)/project/*.h) 
project_srcs := $(wildcard $(framework_dir)/project/*.cpp)
project_objs := $(patsubst %.cpp,%.o,$(project_srcs))

$(project_objs) : %.o : %.cpp $(project_incs)
	$(CC) $(CXXFLAGS) -o $@ $<

project : $(core_objs) $(protocol_objs) $(cpplua_objs) $(test_objs) $(service_objs) $(project_objs)
	$(CC) -o project $^ $(LDFLAGS)

#### phony ####
.PHONY : clean install rebuild

clean:
	-rm -f $(core_gch)
	-rm -f $(test_gch)
	-rm -f $(service_gch)
	-rm -f $(core_objs) $(protocol_objs) $(cpplua_objs) $(test_objs) $(service_objs) $(project_objs)
	-rm -f project
	-rm -f ../bin/project

install:
	-cp -f project $(bindir)

rebuild : clean project

