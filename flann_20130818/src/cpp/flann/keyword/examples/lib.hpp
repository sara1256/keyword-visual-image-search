#ifndef LIB_HPP
#define LIB_HPP

enum ACCESS_TYPE {
	SERIAL_ACCESS = 0,
	RANDOM_ACCESS,
};

void display_filter_info(bloom_filter *filter)
{
	std::cout << "salt_count_                         : " << filter->salt_count_ << std::endl;
	std::cout << "table_size_                         : " << filter->table_size_ << std::endl;
	std::cout << "raw_table_size_                     : " << filter->raw_table_size_ << std::endl;
	std::cout << "projected_element_count_            : " << filter->projected_element_count_ << std::endl;
	std::cout << "inserted_element_count_             : " << filter->inserted_element_count_ << std::endl;
	std::cout << "random_seed_                        : " << filter->random_seed_ << std::endl;
	std::cout << "desired_false_positive_probability_ : " << filter->desired_false_positive_probability_ << std::endl;
	std::cout << "salt_                               : ";

	for (int m=0; m<filter->salt_.size(); m++) std::cout << std::hex << filter->salt_[m] << std::dec; std::cout << std::endl;

	std::cout << "bit_table_                          : ";

	for (int m=0; m<filter->raw_table_size_; m++) std::cout << std::hex << filter->bit_table_[m] << std::dec; std::cout << std::endl;

}

void filter_test(bloom_filter_manager &mgr, ACCESS_TYPE access_type, std::vector<std::string> query_words, bool verbose)
{
	std::cout << "number_of_filters                   : " << mgr.size() << std::endl;

	for (int m=0; m<mgr.size(); m++)
	{
		std::cout << "\nfilter id                           : " << m << std::endl;
	
		bloom_filter *filter;

		switch( access_type )
		{
		case SERIAL_ACCESS:
			filter = &mgr[m];
			break;
		case RANDOM_ACCESS:
			filter = mgr.load_for_random_access(m);
			break;
		default:
			return;
		}	

		if (verbose) display_filter_info(filter);

		std::vector<std::string>::iterator iter = filter->contains_all( query_words.begin(), query_words.end() );
		if (query_words.end() != iter) std::cout << "result                              : key " << (*iter) << " does not found in a bloom filter." << std::endl;
		else std::cout << "result                              : key exists." << std::endl;
	}
}

#endif
