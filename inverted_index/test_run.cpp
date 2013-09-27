#include "inverted_index.hpp"

int main(int argc, char* argv[])
{
	InvertedIndex inv_index;

	std::vector<std::string> new_terms;
	new_terms.push_back("orange");
	new_terms.push_back("kiwi");
	new_terms.push_back("apple");
	new_terms.push_back("juice");

	inv_index.append_terms( new_terms );
	inv_index.append_posting( "apple", 10 );
	inv_index.append_posting( "apple", 12 );
	inv_index.append_posting( "juice", 2 );
	inv_index.append_posting( "juice", 9 );
	inv_index.append_posting( "juice", 53 );
	inv_index.append_posting( "kiwi", 7 );
	inv_index.append_posting( "orange", 17 );
	//inv_index.append_posting( "mango", 59 );
	//inv_index.append_posting( "pimang", 35 );

	inv_index.save( "test.idx" );

	inv_index.open( "test.idx" );

	std::vector< posting_t > posting = inv_index.get_posting_list( "juice" );

	for (int k=0; k<posting.size(); k++) std::cout << "id = " << posting[k] << std::endl;

	inv_index.close();
}
