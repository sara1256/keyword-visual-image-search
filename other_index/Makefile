CC = g++ -std=c++0x
LINKER = g++ -std=c++0x
PORTER2_STEMMER_LIB		= -L../porter2_stemmer/ -lporter2_stemmer
PORTER2_STEMMER_INCLUDE = -I../porter2_stemmer

BOOST_LIB				= -lboost_serialization
BOOST_INCLUDE			= -I/usr/include/boost/

HDF5_LIB				= -lhdf5

FLANN_LIB				= -L../flann/build/lib -lflann_cpp
FLANN_INCLUDE			= -I../flann/src/cpp

BLOOM_INCLUDE			= -I../flann/src/cpp/flann/

INVERTED_INDEX_INCLUDE	= -I../inverted_index/

all: visual_index_andthen_keyword_scan inverted_index_andthen_visual_scan

visual_index_andthen_keyword_scan: visual_index_andthen_keyword_scan.cpp
	$(CC) -o $@ visual_index_andthen_keyword_scan.cpp -L/usr/lib $(HDF5_LIB) $(FLANN_LIB) $(PORTER2_STEMMER_LIB) $(BOOST_LIB) $(FLANN_INCLUDE) $(PORTER2_STEMMER_INCLUDE) $(BOOST_INCLUDE) $(BLOOM_INCLUDE)


inverted_index_andthen_visual_scan: inverted_index_andthen_visual_scan.cpp
	$(CC) -o $@ inverted_index_andthen_visual_scan.cpp -L/usr/lib $(HDF5_LIB) $(FLANN_LIB) $(PORTER2_STEMMER_LIB) $(BOOST_LIB) $(FLANN_INCLUDE) $(PORTER2_STEMMER_INCLUDE) $(BOOST_INCLUDE) $(INVERTED_INDEX_INCLUDE)

clean:
	rm visual_index_andthen_keyword_scan inverted_index_andthen_visual_scan
