#ifndef BLOOM_FILTER_MANAGER_HPP
#define BLOOM_FILTER_MANAGER_HPP

#include "bloom_filter.hpp"

class bloom_filter_manager
{
private:
	friend class boost::serialization::access;

	std::vector<bloom_filter *> filters;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & filters;
	}

public:
	bloom_filter_manager() { }

	void append(const bloom_filter &f)
	{
		filters.push_back( new bloom_filter(f) );
	}

	void save(const char *filename)
	{
		std::ofstream ofs(filename);
		boost::archive::binary_oarchive oa(ofs);
		oa << *this;
	}

	void load(const char *filename)
	{
		std::ifstream ifs(filename);
		boost::archive::binary_iarchive ia(ifs);
		ia >> *this;
	}

	bloom_filter& operator[](int index)
	{
		assert(index >= 0 && index < filters.size());
		return *filters[index];
	}

	void test()
	{
		std::vector<std::string> keywords;
		keywords.push_back("apple");
		keywords.push_back("banana");

		std::cout << "number of filters: " << filters.size() << std::endl;

		for (unsigned int k=0; k<filters.size(); k++)
		{
			std::vector<std::string>::iterator iter = filters[k]->contains_all(keywords.begin(), keywords.end());

            std::cout << "\nsalt_count_: " << filters[k]->salt_count_ << std::endl;;
            std::cout << "table_size_: " << filters[k]->table_size_<< std::endl;;
            std::cout << "raw_table_size_: " << filters[k]->raw_table_size_<< std::endl;;
            std::cout << "projected_element_count_: " << filters[k]->projected_element_count_<< std::endl;;
            std::cout << "inserted_element_count_: " << filters[k]->inserted_element_count_<< std::endl;;
			std::cout << "random_seed_: " << filters[k]->random_seed_<< std::endl;;
			std::cout << "desired_false_positive_probability_: " << filters[k]->desired_false_positive_probability_<< std::endl;;

			if (keywords.end() != iter) std::cout << "filter: key " << (*iter) << " not found in bloom filter." << std::endl;
			else std::cout << "filter: key exists." << std::endl;
		} 
	}
};

#endif
