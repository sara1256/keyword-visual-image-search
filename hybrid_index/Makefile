CC = g++ -std=c++0x
LINKER = g++ -std=c++0x
PORTER2_STEMMER_LIB		= -L../porter2_stemmer/
BOOST_INCLUDE			= -I/usr/include/boost/

FLANN_LIB				= -L../flann/build/lib -lflann_cpp
FLANN_INCLUDE			= -I../flann/src/cpp


all: indexer
	$(CC) flann_example.cpp -L/usr/lib -lhdf5 -L../flann/build/lib -lflann_cpp $(PORTER2_STEMMER_LIB) -lporter2_stemmer -lboost_serialization -I../flann/src/cpp -I../porter2_stemmer $(BOOST_INCLUDE)

indexer: indexer.cpp
	$(CC) -o $@ indexer.cpp -L/usr/lib -lhdf5 $(FLANN_LIB) -lboost_serialization $(FLANN_INCLUDE) $(BOOST_INCLUDE)

clean:
	rm indexer *.out core
