CC = g++ -std=c++0x
LINKER = g++ -std=c++0x
PORTER2_STEMMER = ../../../porter2_stemmer

all:
	$(CC) -o stemmer stemmer.cpp -L$(PORTER2_STEMMER) -lporter2_stemmer -I$(PORTER2_STEMMER)

clean:
	rm stemmer
