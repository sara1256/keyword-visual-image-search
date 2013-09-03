/**
 * @author Tak-Eun Kim
 * @date August 2013
 *
 */

#include <iostream>
#include <fstream>
#include <string>
#include <porter2_stemmer.h>

int main(int argc, char* argv[])
{
	if (argc != 3) {
		std::cerr << "Usage: ./stemmer [input_file] [output_file]" << std::endl;		
		return -1;
	}

	std::ifstream in(argv[1]);
	std::ofstream out(argv[2]);

	if (!in.is_open()) { std::cerr << "Unable to open an input file : " << argv[1] << std::endl; return -1; }
	if (!out.is_open()) { std::cerr << "Unable to open an output file : " << argv[2] << std::endl; return -1; }

	std::string word_orig;
	std::string word_stem;

	while (in >> word_orig)
	{
		word_stem = Porter2Stemmer::stem(word_orig);
		out << word_stem << "\n";
	}

	in.close();
	out.close();

	return 0;
}
