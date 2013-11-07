#include <cassert>
#include <fstream>
#include <sstream>
#include <iostream>

#include <sys/time.h>

#include "bloom_filter_manager.hpp"
#include "lib.hpp"

double get_elapsed_time_in_msecs( const timeval t1, const timeval t2 )
{
	long elapsed_secs = t2.tv_sec - t1.tv_sec;
	long elapsed_usecs = t2.tv_usec - t1.tv_usec;

	return elapsed_secs * 1000.0 + elapsed_usecs / 1000.0;
}

bool load_access_pattern( const std::string filename, std::vector<int> &patterns )
{
	std::ifstream ifs( filename );
	if ( !ifs.is_open() )
	{
		std::cerr << "Unable to open an access pattern file : " << filename << std::endl;
		return false;
	}

	std::string buffer;
	int access_position;

	while (std::getline(ifs, buffer))
	{
		std::istringstream ss( buffer );
		ss >> access_position;

		patterns.push_back( access_position );
	}

	return true;
}

int main(int argc, char* argv[])
{
	//std::string base_path = "/media/mojool1984/research_storage/mirflickr1M_dataset/working/integrated/";
	std::string base_path = "/home/mojool1984/Desktop/project/keyword-visual-image-search/hybrid_index/";
	std::string signature_path = base_path + "merged_tags_for_random_access.dat";

	bloom_filter_manager bf;

	assert( bf.open_for_random_access( signature_path.c_str() ) );

	std::vector<int> patterns;
	std::string filename = "/home/mojool1984/Desktop/project/keyword-visual-image-search/hybrid_index/t_berlin_red";
	assert( load_access_pattern( filename, patterns ) );

	timeval start_time, end_time; 
	
	gettimeofday(&start_time, 0);

	//std::sort( patterns.begin(), patterns.end() );

	for (int k=0; k<patterns.size(); k++)
	{
		std::cout << "Processing " << patterns[k] << std::endl;
		bf.load_for_random_access( patterns[k] );
	}

	gettimeofday(&end_time, 0);

	std::cout << "\nElapsed msecs = " << get_elapsed_time_in_msecs( start_time, end_time ) << std::endl;

	bf.close_for_random_access();
}
