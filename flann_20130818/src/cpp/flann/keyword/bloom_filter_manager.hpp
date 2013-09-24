#ifndef BLOOM_FILTER_MANAGER_HPP
#define BLOOM_FILTER_MANAGER_HPP

#include "bloom_filter.hpp"

/**
  * Added by mojool
  */
struct bloom_filter_header_t
{
	unsigned int            salt_count_;
	unsigned long long int  table_size_;
	unsigned long long int  raw_table_size_;
	unsigned long long int  projected_element_count_;
	unsigned int            inserted_element_count_;
	unsigned long long int  random_seed_;
	double                  desired_false_positive_probability_;
};

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

	std::ifstream ifs_for_random_access;

	/**
	  * global settings for filters
	  */
	typedef unsigned int bloom_type;
	typedef unsigned char cell_type;

	unsigned int number_of_filters_;
	bloom_filter_header_t header_;
	std::vector<bloom_type> salt_;
	std::streampos header_size_;		// header size (bytes)
	
public:
	bloom_filter_manager(): number_of_filters_(0) { }

	int append(const bloom_filter &f)
	{
		filters.push_back( new bloom_filter(f) );
		return filters.size() - 1;
	}

	void save_for_serial_access(const char *filename)
	{
		std::ofstream ofs(filename);
		boost::archive::binary_oarchive oa(ofs);
		oa << *this;
	}

	void load_for_serial_access(const char *filename)
	{
		std::ifstream ifs(filename);
		boost::archive::binary_iarchive ia(ifs);
		ia >> *this;
	}


	/**
	  * I/O for random access
	  */

	bool open_for_random_access(const char *filename)
	{
		ifs_for_random_access.open( filename, std::ios::in | std::ios::binary );
		if (!ifs_for_random_access.is_open()) 
		{
			std::cerr << "Unable to open a file : " << filename << std::endl;
			return false;
		}

		// load total number of filters
		ifs_for_random_access.read( reinterpret_cast<char*>(&number_of_filters_), sizeof(number_of_filters_) );

		// load bloom_filter parameters (except salt_ & bit_table_)
		ifs_for_random_access.read( reinterpret_cast<char*>(&header_), sizeof(bloom_filter_header_t) );

		// load salt_
		salt_.resize(header_.salt_count_);
		ifs_for_random_access.read( reinterpret_cast<char*>(&salt_[0]), sizeof(bloom_type) * header_.salt_count_ );

		header_size_ = ifs_for_random_access.tellg();
		ifs_for_random_access.seekg( header_size_, std::ios_base::beg );

		return true;
	}

	bloom_filter* load_for_random_access(unsigned int index)
	{
		assert( index >= 0 && index < number_of_filters_ );

		// locate the file curser at the end of header
		ifs_for_random_access.seekg( header_size_, std::ios_base::beg );

		// load a bit_table_ of a specified filter
		ifs_for_random_access.seekg( index * header_.raw_table_size_, std::ios_base::cur );

		// set header parameters to a new filter
		bloom_filter *filter = new bloom_filter();

		filter->salt_count_ = header_.salt_count_;
		filter->table_size_ = header_.table_size_;
		filter->raw_table_size_ = header_.raw_table_size_;
		filter->projected_element_count_ = header_.projected_element_count_;
		filter->inserted_element_count_ = header_.inserted_element_count_;
		filter->random_seed_ = header_.random_seed_;
		filter->desired_false_positive_probability_ = header_.desired_false_positive_probability_;

		// copy salt_ to a new filter
		filter->salt_.resize( salt_.size() );
		std::copy( salt_.begin(), salt_.end(), filter->salt_.begin() );

		// load bit_table_ in a disk into memory
		filter->bit_table_ = new cell_type[static_cast<std::size_t>(filter->raw_table_size_)];
		ifs_for_random_access.read( reinterpret_cast<char*>(filter->bit_table_),
									sizeof(unsigned char) * filter->raw_table_size_);

		return filter;
	}

	void save_for_random_access(const char *filename)
	{
		std::ofstream ofs(filename, std::ios::out | std::ios::binary);

		bloom_filter_header_t h;

		bloom_filter *bf = filters[0];

		h.salt_count_ = bf->salt_count_;
		h.table_size_ = bf->table_size_;
		h.raw_table_size_ = bf->raw_table_size_;
		h.projected_element_count_ = bf->projected_element_count_;
		h.inserted_element_count_ = bf->inserted_element_count_;
		h.random_seed_ = bf->random_seed_;
		h.desired_false_positive_probability_ = bf->desired_false_positive_probability_;

		number_of_filters_ = filters.size();
		ofs.write( reinterpret_cast<char*>(&number_of_filters_), sizeof(number_of_filters_) );
		ofs.write( reinterpret_cast<char*>(&h), sizeof(bloom_filter_header_t) );
		ofs.write( reinterpret_cast<char*>(&bf->salt_[0]), sizeof(bloom_type) * bf->salt_.size() );
	
		for (unsigned int k=0; k<filters.size(); k++)
			ofs.write( reinterpret_cast<char*>(filters[k]->bit_table_),
						sizeof(unsigned char) * filters[k]->raw_table_size_ );

		ofs.close();
	}

	void close_for_random_access()
	{
		if (ifs_for_random_access.is_open()) ifs_for_random_access.close();
	}

	bloom_filter& operator[](int index) const
	{
		assert(index >= 0 && index < filters.size());
		return *filters[index];
	}

	size_t size() const
	{
		return std::max( (int)filters.size(), (int)number_of_filters_ );
	}
};

#endif
