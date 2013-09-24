#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

#include "bloom_filter_manager.hpp"
#include "lib.hpp"

int main(int argc, char* argv[])
{
	std::string filename1 = "result_serial_access.dat";
	std::string filename2 = "result_random_access.dat";

	//---------- set lists of keywords ----------//
	std::vector<std::string> word_list1, word_list2;

	word_list1.push_back("apple");
	word_list1.push_back("banana");

	word_list2.push_back("coconut");
	word_list2.push_back("kiwi");

	//---------- query keywords ----------//
	std::vector<std::string> query_words;
	query_words.push_back("apple");
	query_words.push_back("banana");

	//---------- set bloom parameters ----------//
	bloom_parameters param;

	param.projected_element_count = 1000;
	param.false_positive_probability = 1.0 / 100;
	param.random_seed = 1;

	param.compute_optimal_parameters();

	//---------- create filters ----------//
	bloom_filter filter1(param);
	filter1.insert(word_list1.begin(), word_list1.end());

	bloom_filter filter2(param);
	filter2.insert(word_list2.begin(), word_list2.end());

	//---------- create bloom filter manager ----------//
	bloom_filter_manager mgr_for_save, mgr_for_load;

	mgr_for_save.append( filter1 );
	mgr_for_save.append( filter2 );

	//---------- test for boost serialization ----------//
	std::cout << "\n------------------------------------------------------------------" << std::endl;
	std::cout << "|                     BOOST SERIALIZATION TEST                   |" << std::endl;
	std::cout << "------------------------------------------------------------------\n" << std::endl;

	mgr_for_save.save_for_serial_access( filename1.c_str() );
	mgr_for_load.load_for_serial_access( filename1.c_str() );
	filter_test( mgr_for_load, SERIAL_ACCESS, query_words, true );

	//---------- test for random access ----------//
	std::cout << "\n------------------------------------------------------------------" << std::endl;
	std::cout << "|                       RANDOM ACCESS TEST                       |" << std::endl;
	std::cout << "------------------------------------------------------------------\n" << std::endl;

	mgr_for_save.save_for_random_access( filename2.c_str() );
	assert( mgr_for_load.open_for_random_access( filename2.c_str() ) );
	filter_test( mgr_for_load, RANDOM_ACCESS, query_words, true );

	//---------- close all file streams ----------//
	mgr_for_save.close_for_random_access();
	mgr_for_load.close_for_random_access();

	return 0;
}
