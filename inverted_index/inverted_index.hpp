#ifndef INVERTED_INDEX_HPP
#define INVERTED_INDEX_HPP

#include <algorithm>
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

typedef unsigned int posting_t;

class InvertedIndex
{
public:
	InvertedIndex() : loaded_(false)
	{
	}

	~InvertedIndex()
	{
	}

	bool save( const char *filename )
	{
		std::ofstream ofs( filename, std::ios::out | std::ios::binary );
		save_terms( ofs );
		save_postings( ofs );
		ofs.close();

		return true;
	}

	bool open( const char *filename )
	{
		ifs.open( filename, std::ios::in | std::ios::binary );
		if (!ifs.is_open()) return false;

		load_terms();
		load_postings_header();

		loaded_ = true;

		return true;
	}

	void append( std::string term, posting_t post )
	{
		// check existance of the term
		if ( inv_index.find( term ) == inv_index.end() )	// not found
		{
			std::vector<posting_t> posting; posting.push_back( post );
			std::pair<std::string, std::vector<posting_t>> entry( term, posting );
			inv_index.insert( entry );
		}
		else // found
		{
			inv_index.at( term ).push_back( post );
		}
	}

	//---------- getters ----------//
	std::vector<posting_t> operator[]( std::string term )
	{
		if ( !load_postings( term ) ) return *(new std::vector<posting_t>);
		//load_postings( term );

		if ( inv_index.find( term ) == inv_index.end() )
			return *(new std::vector<posting_t>);

		return inv_index.at( term );
	}

	std::vector<posting_t> find( std::vector<std::string> terms )
	{
		if (terms.size() == 1) return (*this)[terms[0]];

		// intersect posting lists
		std::vector<posting_t> first = (*this)[terms[0]];

		for (int m=1; m<terms.size(); m++)
		{
			std::cout << "==> " << terms[m] << std::endl;
			std::vector<posting_t> second = (*this)[terms[m]];
	
			first = merge( first, second );
		}

		return first;
	}

	size_t get_num_terms()
	{
		assert( loaded_ );
		return num_terms;
	}

	void close()
	{
		if (ifs.is_open()) ifs.close();
	}

private:
	size_t num_terms;
	std::vector<size_t> posting_lengths;
	std::vector<size_t> accumulative_posting_lengths;
	bool loaded_;

	std::unordered_map<std::string, std::vector<posting_t>> inv_index;

	// the following variable keeps track of a term and its saving order in a file.
	std::unordered_map<std::string, unsigned int> term_saving_orders;

	std::ifstream ifs;

	std::streampos header_size_;	// header = term section + posting header

	static bool comparator(posting_t p1, posting_t p2)
	{
		return p1 < p2;
	}

	void load_terms()
	{
		ifs.read( reinterpret_cast<char*>(&num_terms), sizeof(num_terms) );

		for (size_t k=0; k<num_terms; k++)
		{
			size_t term_size;

			ifs.read( reinterpret_cast<char*>(&term_size), sizeof(size_t) );

			std::vector<char> tmp( term_size );
			ifs.read(&tmp[0], term_size);

			std::string term;
			term.assign(&tmp[0], term_size);

			std::pair<std::string, unsigned int> entry( term, (unsigned int)k );
			term_saving_orders.insert( entry );
		}
	}

	void save_terms( std::ofstream& stream )
	{
		size_t num_terms = inv_index.size();
		stream.write( reinterpret_cast<char*>(&num_terms), sizeof(size_t) );

		for (auto it = inv_index.cbegin(); it != inv_index.cend(); ++it)
		{
			std::string term = it->first;
			size_t term_size = term.size();

			stream.write( reinterpret_cast<char*>(&term_size), sizeof(size_t) );

			stream << term;
		}
	}

	void load_postings_header()
	{
		if (num_terms == 0) return;

		posting_lengths.resize( num_terms );
		accumulative_posting_lengths.resize( num_terms );
		accumulative_posting_lengths[0] = 0;

		for (size_t k=0; k<num_terms-1; k++)
		{
			ifs.read( reinterpret_cast<char*>(&posting_lengths[k]), sizeof(size_t) );
			accumulative_posting_lengths[k+1] = accumulative_posting_lengths[k] + posting_lengths[k];
		}

		ifs.read( reinterpret_cast<char*>(&posting_lengths[num_terms-1]), sizeof(size_t) );

		header_size_ = ifs.tellg();
	}

	bool load_postings( std::string term )
	{
		auto it = term_saving_orders.find( term );
	
		// if term doesn't exist, return nothing	
		if ( it == term_saving_orders.end() ) return false;

		// locate the file cursor at the end of header
		ifs.seekg( header_size_, std::ios_base::beg );

		size_t pos = accumulative_posting_lengths[it->second] * sizeof(posting_t);
		ifs.seekg( (std::streampos)pos, std::ios_base::cur );

		// read and assign postings into vectors
		std::vector<posting_t> posting;
		posting.resize( posting_lengths[it->second] );

		ifs.read( reinterpret_cast<char*>(&posting[0]), sizeof(posting_t) * posting_lengths[it->second] );

		std::pair<std::string, std::vector<posting_t>> entry( term, posting );
		inv_index.insert( entry );
		return true;
	}

	void save_postings( std::ofstream& stream )
	{
		for (auto it = inv_index.cbegin(); it != inv_index.cend(); ++it)
		{
			std::vector<posting_t> posting = it->second;
			size_t posting_size = posting.size();
			stream.write( reinterpret_cast<char*>(&posting_size), sizeof(size_t) );
		}

		for (auto it = inv_index.cbegin(); it != inv_index.cend(); ++it)
		{
			std::vector<posting_t> posting = it->second;
			stream.write( reinterpret_cast<char*>(&posting[0]), sizeof(posting_t) * posting.size() );
		}
	}

	std::vector<posting_t> merge( const std::vector<posting_t> first,
								  const std::vector<posting_t> second)
	{
		std::vector<posting_t> result;
		result.resize( first.size() + second.size() );

		auto it = std::set_intersection( first.begin(), first.end(), second.begin(), second.end(), result.begin() );
		result.resize(it - result.begin());
		return result;
	}


};

#endif
