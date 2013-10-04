#include "inverted_index.hpp"
#include <unordered_map>

/*
void testcase_merge()
{
	InvertedIndex inv_index1, inv_index2;

	std::vector<std::string> terms1, terms2;

	terms1.push_back("orange");
	terms1.push_back("kiwi");
	terms1.push_back("apple");
	terms1.push_back("juice");

	terms2.push_back("lemon");
	terms2.push_back("kiwi");
	terms2.push_back("apple");
	terms2.push_back("mango");

	inv_index1.append_terms( terms1 );
	inv_index2.append_terms( terms2 );

	inv_index1.append_posting( "apple", 10 );
	inv_index1.append_posting( "apple", 12 );
	inv_index1.append_posting( "juice", 2 );
	inv_index1.append_posting( "juice", 9 );
	inv_index1.append_posting( "juice", 53 );
	inv_index1.append_posting( "kiwi", 7 );
	inv_index1.append_posting( "orange", 17 );

	inv_index2.append_posting( "lemon", 559 );
	inv_index2.append_posting( "lemon", 326 );
	inv_index2.append_posting( "mango", 96 );
	inv_index2.append_posting( "kiwi", 29 );
	inv_index2.append_posting( "kiwi", 93 );

	InvertedIndex inv_index3;
}
*/

int main(int argc, char* argv[])
{
	InvertedIndex inv_index;

	/*
	std::vector<std::string> new_terms;
	new_terms.push_back("orange");
	new_terms.push_back("kiwi");
	new_terms.push_back("apple");
	new_terms.push_back("juice");

	//inv_index.append_terms( new_terms );
	inv_index.append( "apple", 10 );
	inv_index.append( "apple", 12 );
	inv_index.append( "juice", 2 );
	inv_index.append( "juice", 9 );
	inv_index.append( "juice", 53 );
	inv_index.append( "kiwi", 7 );
	inv_index.append( "orange", 17 );
	inv_index.append( "mango", 59 );
	inv_index.append( "pimang", 35 );

	inv_index.save( "test.idx" );
	*/

	inv_index.open( "test.idx" );

	//std::vector< posting_t > posting = inv_index["mango"];
	std::vector<std::string> terms = {{"mango"}, {"apple"}};
	std::vector< posting_t > posting = inv_index.find( terms );

	for (int k=0; k<posting.size(); k++) std::cout << "id = " << posting[k] << std::endl;

	inv_index.close();

	//testcase_merge();
}
