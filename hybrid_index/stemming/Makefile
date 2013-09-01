CC = g++ -std=c++0x
LINKER = g++ -std=c++0x

all:
	$(CC) -o stemmer stemmer.cpp -L../../porter2_stemmer -lporter2_stemmer -I../../porter2_stemmer

clean:
	rm stemmer
