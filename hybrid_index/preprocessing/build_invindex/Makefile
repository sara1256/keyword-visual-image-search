CC = g++ -std=c++0x
LINKER = g++ -std=c++0x
INVERTED_INDEX_INCLUDE	= -I../../../inverted_index/

all: inv_indexer inv_searcher

inv_indexer: inv_indexer.cpp
	$(CC) -o $@ inv_indexer.cpp $(INVERTED_INDEX_INCLUDE)

inv_searcher: inv_searcher.cpp
	$(CC) -o $@ inv_searcher.cpp $(INVERTED_INDEX_INCLUDE)

clean:
	rm inv_indexer inv_searcher
