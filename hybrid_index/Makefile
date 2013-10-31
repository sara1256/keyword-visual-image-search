CC = g++ -std=c++0x
LINKER = g++ -std=c++0x
PORTER2_STEMMER_LIB		= -L../porter2_stemmer/ -lporter2_stemmer
PORTER2_STEMMER_INCLUDE = -I../porter2_stemmer

BOOST_LIB				= -lboost_serialization
BOOST_INCLUDE			= -I/usr/include/boost/

HDF5_LIB				= -lhdf5

FLANN_LIB				= -L../flann/build/lib -lflann_cpp
FLANN_INCLUDE			= -I../flann/src/cpp


all: hybrid_indexer hybrid_searcher

hybrid_indexer: hybrid_indexer.cpp
	$(CC) -o $@ hybrid_indexer.cpp -L/usr/lib $(HDF5_LIB) $(FLANN_LIB) $(BOOST_LIB) $(FLANN_INCLUDE) $(BOOST_INCLUDE)

hybrid_searcher: hybrid_searcher.cpp
	$(CC) -o $@ hybrid_searcher.cpp -L/usr/lib $(HDF5_LIB) $(FLANN_LIB) $(PORTER2_STEMMER_LIB) $(BOOST_LIB) $(FLANN_INCLUDE) $(PORTER2_STEMMER_INCLUDE) $(BOOST_INCLUDE) -I../indices/common/

clean:
	rm hybrid_indexer hybrid_searcher
