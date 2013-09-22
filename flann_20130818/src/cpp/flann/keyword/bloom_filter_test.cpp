#include <iostream>
#include <algorithm>
#include <vector>

#include "bloom_filter_manager.hpp"

int main(int argc, char* argv[])
{
	std::string filename = "test.dat";

	std::vector<std::string> word_list1, word_list2;

	word_list1.push_back("apple");
	word_list1.push_back("banana");

	word_list2.push_back("coconut");
	word_list2.push_back("kiwi");

	bloom_parameters param;

	//param.projected_element_count = 1980;
	param.projected_element_count = 10000;
	//param.false_positive_probability = 1.0 / 1980;
	param.false_positive_probability = 1.0 / 100;
	param.random_seed = 1;

	param.compute_optimal_parameters();

	bloom_filter filter1(param);
	filter1.insert(word_list1.begin(), word_list1.end());

	bloom_filter filter2(param);
	filter2.insert(word_list2.begin(), word_list2.end());

	bloom_filter_manager mgr;
	mgr.append(filter1);
	mgr.append(filter2);

	std::cout << "========== before save ==========" << std::endl;
	mgr.test();
	mgr.save(filename.c_str());

	std::cout << "\n========== after load ==========" << std::endl;
	bloom_filter_manager mgr_loaded;
	mgr_loaded.load(filename.c_str());
	mgr_loaded.test();

	bloom_filter bf = mgr_loaded[1];

	/*
	std::cout << "\nsalt_count_: " << bf.salt_count_ << std::endl;;
	std::cout << "table_size_: " << bf.table_size_<< std::endl;;
	std::cout << "raw_table_size_: " << filters[k]->raw_table_size_<< std::endl;;
	std::cout << "projected_element_count_: " << filters[k]->projected_element_count_<< std::endl;;
	std::cout << "inserted_element_count_: " << filters[k]->inserted_element_count_<< std::endl;;
	std::cout << "random_seed_: " << filters[k]->random_seed_<< std::endl;;
	std::cout << "desired_false_positive_probability_: " << filters[k]->desired_false_positive_probability_<< std::endl;;

	if (keywords.end() != iter) std::cout << "filter: key " << (*iter) << " not found in bloom filter." << std::endl;
	else std::cout << "filter: key exists." << std::endl;
	*/

	return 0;
}
