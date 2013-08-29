CC = g++ -std=c++0x
LINKER = g++ -std=c++0x

all:
	$(CC) flann_example.cpp -L/usr/lib -lhdf5 -L../build/lib -lflann_cpp -L../../porter2_stemmer -lporter2_stemmer -I../src/cpp -I../../porter2_stemmer
