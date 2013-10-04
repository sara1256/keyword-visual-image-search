CC = g++ -std=c++0x
LINKER = g++ -std=c++0x

all: test_run

test_run: test_run.cpp inverted_index.hpp
	$(CC) -o $@ test_run.cpp

clean:
	rm test_run test.idx
