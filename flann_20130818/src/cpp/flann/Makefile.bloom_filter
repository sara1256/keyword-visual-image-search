COMPILER		= -c++
OPT				= -L/usr/lib/
BOOST_LIB		= -lboost_serialization
BOOST_INCLUDE	= -I/usr/include/boost

all:
	$(COMPILER) -o bloom_filter_test bloom_filter_test.cpp $(OPT) $(BOOST_LIB) $(BOOST_INCLUDE)

clean:
	rm bloom_filter_test test.dat
