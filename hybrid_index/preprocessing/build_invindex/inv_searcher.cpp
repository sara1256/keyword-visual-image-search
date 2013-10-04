#include "inverted_index.hpp"
#include <sys/time.h>

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: ./inv_searcher keyword_1 keyword_2 ... keyword_n" << std::endl;
		return -1;
	}

	InvertedIndex inv_index;

	inv_index.open( "tmp/mirflickr1M_invindex.idx" );

	timeval start, end;

	std::vector<std::string> terms;
	for (int m=1; m<argc; m++) terms.push_back( argv[m] );

	gettimeofday(&start, 0);

	std::vector<posting_t> posting = inv_index.find( terms );

	gettimeofday(&end, 0);

	long seconds = end.tv_sec - start.tv_sec;
	long useconds = end.tv_usec - start.tv_usec;

	std::cout << "Elapsed msecs = " << (seconds * 1000.0 + useconds/1000.0) << std::endl;

	for (int m=0; m<posting.size(); m++)
		std::cout << posting[m] << " ";
	std::cout << std::endl;

	inv_index.close();

	return 0;
}
