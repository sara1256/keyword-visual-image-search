#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

#include "bloom_filter_manager.hpp"
#include "lib.hpp"

int main(int argc, char* argv[])
{
	//---------- query keywords ----------//
	std::vector<std::string> query_words;
	query_words.push_back("tattoo");
	//query_words.push_back("libstick");
	//query_words.push_back("sunglass");

	//---------- create bloom filter manager ----------//
	bloom_filter_manager mgr_for_load;

	//---------- test for boost serialization ----------//
	std::cout << "\n------------------------------------------------------------------" << std::endl;
	std::cout << "|            TEST FOR RANDOM ACCESS OF LARGE DATA FILE           |" << std::endl;
	std::cout << "------------------------------------------------------------------\n" << std::endl;

	//---------- large file load ----------//
	std::string filename = "/media/mojool1984/research_storage/mirflickr1M_dataset/working/integrated/merged_tags_for_random_access.dat";
	//std::string filename = "/home/mojool1984/Desktop/project/keyword-visual-image-search/hybrid_index/nonleaf_ok.dat";	

	assert( mgr_for_load.open_for_random_access( filename.c_str() ) );
	filter_test( mgr_for_load, RANDOM_ACCESS, query_words, false );

	//---------- close all file streams ----------//
	mgr_for_load.close_for_random_access();

	return 0;
}
