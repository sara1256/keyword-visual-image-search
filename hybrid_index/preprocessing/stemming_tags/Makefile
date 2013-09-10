CC 					= g++ -std=c++0x

INC					= ../../../porter2_stemmer
OPT					= -L/usr/lib/

BOOST_LIB			= -lboost_serialization -lboost_system -lboost_filesystem
BOOST_INC			= -I/usr/include/boost

BLOOM_INC			= -I../../../flann/src/cpp/flann/

YAML_INC			= -I../../../3rd_party/yaml-cpp-0.5.1/include/
YAML_LIB_PATH		= -L../../../3rd_party/yaml-cpp-0.5.1/build/
YAML_LIB			= -lyaml-cpp

all: stemmer merger

stemmer: stemmer.cpp
	$(CC) -o stemmer stemmer.cpp -L$(INC) -lporter2_stemmer -I$(INC)

merger: merger.cpp
	$(CC) -o merger merger.cpp $(OPT) $(BOOST_LIB) $(BOOST_INC) $(BLOOM_INC)

clean:
	rm stemmer merger
