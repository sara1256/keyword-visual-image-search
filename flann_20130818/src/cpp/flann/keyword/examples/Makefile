COMPILER				= -g++ -std=c++0x
OPT						= -L/usr/lib/
BOOST_LIB				= -lboost_serialization
BOOST_INCLUDE			= -I/usr/include/boost
BLOOM_FILTER_INCLUDE	= -I../

all: test_access_mode test_large_data_access test_disk_io

test_access_mode: test_access_mode.cpp
	$(COMPILER) -o $@ test_access_mode.cpp $(OPT) $(BOOST_LIB) $(BOOST_INCLUDE) $(BLOOM_FILTER_INCLUDE)

test_large_data_access: test_large_data_access.cpp
	$(COMPILER) -o $@ test_large_data_access.cpp $(OPT) $(BOOST_LIB) $(BOOST_INCLUDE) $(BLOOM_FILTER_INCLUDE)

test_disk_io: test_disk_io.cpp
	$(COMPILER) -o $@ test_disk_io.cpp $(OPT) $(BOOST_LIB) $(BOOST_INCLUDE) $(BLOOM_FILTER_INCLUDE)

clean:
	rm test_access_mode test_large_data_access test_disk_io
	rm result_serial_access.dat result_random_access.dat
