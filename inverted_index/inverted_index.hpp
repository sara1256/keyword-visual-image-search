#ifndef INVERTED_INDEX_HPP
#define INVERTED_INDEX_HPP

#include <algorithm>
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

typedef unsigned int posting_t;

class InvertedIndex
{
public:
	InvertedIndex() : loaded_(false), postings_loaded_(NULL)
	{
	}

	~InvertedIndex()
	{
		if (postings_loaded_ != NULL) delete[] postings_loaded_;
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

		load_terms( ifs );
		load_postings_header( ifs );

		loaded_ = true;

		return true;
	}

	void append_terms( std::vector<std::string> new_terms )
	{
		terms.resize( new_terms.size() );
		postings.resize( new_terms.size() );
		std::copy( new_terms.begin(), new_terms.end(), terms.begin() );
		std::sort( terms.begin(), terms.end() );

	}

	void append_posting( std::string term, posting_t post )
	{
		std::vector<std::string>::iterator iter = std::find( terms.begin(), terms.end(), term );

		// the term does not exist
		assert( iter != terms.end() );

		unsigned int term_id = iter - terms.begin();
		postings[term_id].push_back(post);
	}

	//---------- getters ----------//
	std::vector< posting_t> get_posting_list( std::string term )
	{
		assert( loaded_ );

		// get term id
		std::vector<std::string>::iterator iter = std::find( terms.begin(), terms.end(), term );

		// the term does not exist
		assert( iter != terms.end() );

		unsigned int term_id = iter - terms.begin();
		load_postings( ifs, term_id );

		std::cout << "size = " << postings[term_id].size() << std::endl;

		return postings[term_id];
	}

	std::vector< posting_t> get_posting_list( unsigned int term_id )
	{
		assert( loaded_ );

		load_postings( ifs, term_id );
		return postings[term_id];
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
	std::vector<std::string> terms;
	std::vector< std::vector<posting_t> > postings;
	std::vector<size_t> posting_lengths;
	std::vector<size_t> accumulative_posting_lengths;
	bool *postings_loaded_;
	bool loaded_;

	std::ifstream ifs;

	std::streampos header_size_;	// header = term section + posting header

	static bool comparator(posting_t p1, posting_t p2)
	{
		return p1 < p2;
	}

	void load_terms( std::ifstream& stream )
	{
		stream.read( reinterpret_cast<char*>(&num_terms), sizeof(num_terms) );

		terms.clear();	
		terms.resize( num_terms );

		for (size_t k=0; k<num_terms; k++)
		{
			size_t term_size;

			stream.read( reinterpret_cast<char*>(&term_size), sizeof(size_t) );

			std::vector<char> tmp( term_size );
			stream.read(&tmp[0], term_size);

			terms[k].assign(&tmp[0], term_size);
		}
	}

	void save_terms( std::ofstream& stream )
	{
		num_terms = terms.size();

		stream.write( reinterpret_cast<char*>(&num_terms), sizeof(size_t) );

		for (size_t k=0; k<num_terms; k++)
		{
			size_t term_size = terms[k].size();
			std::string term = terms[k];

			stream.write( reinterpret_cast<char*>(&term_size), sizeof(size_t) );

			stream << terms[k];
		}
	}

	void load_postings_header( std::ifstream& stream )
	{
		if (num_terms == 0) return;

		posting_lengths.resize( num_terms );
		accumulative_posting_lengths.resize( num_terms );
		accumulative_posting_lengths[0] = 0;

		for (size_t k=0; k<num_terms-1; k++)
		{
			stream.read( reinterpret_cast<char*>(&posting_lengths[k]), sizeof(size_t) );
			accumulative_posting_lengths[k+1] = accumulative_posting_lengths[k] + posting_lengths[k];
		}

		stream.read( reinterpret_cast<char*>(&posting_lengths[num_terms-1]), sizeof(size_t) );

		header_size_ = stream.tellg();

		// set postings_loaded_ array
		postings_loaded_ = new bool[num_terms];
		std::memset( postings_loaded_, false, sizeof(bool) * num_terms );

		// resize postings
		postings.resize( num_terms );
	}

	void load_postings( std::ifstream& stream, unsigned int term_id )
	{
		assert( term_id >= 0 && term_id < num_terms );

		if (postings_loaded_[term_id]) return;

		// locate the file cursor at the end of header
		stream.seekg( header_size_, std::ios_base::beg );

		size_t pos = accumulative_posting_lengths[term_id] * sizeof(posting_t);
		stream.seekg( (std::streampos)pos, std::ios_base::cur );

		// read and assign postings into vectors
		postings[term_id].resize( posting_lengths[term_id] );

		stream.read( reinterpret_cast<char*>(&postings[term_id][0]), sizeof(posting_t) * posting_lengths[term_id] );

		postings_loaded_[term_id] = true;
	}

	void save_postings( std::ofstream& stream )
	{
		for (size_t k=0; k<num_terms; k++)
		{
			size_t posting_size = postings[k].size();
			stream.write( reinterpret_cast<char*>(&posting_size), sizeof(size_t) );
		}

		for (size_t k=0; k<num_terms; k++)
			stream.write( reinterpret_cast<char*>(&postings[k][0]), sizeof(posting_t) * postings[k].size() );
	}

};

#endif
