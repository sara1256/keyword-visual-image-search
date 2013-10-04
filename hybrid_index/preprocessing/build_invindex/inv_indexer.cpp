#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <inverted_index.hpp>

typedef struct
{
	unsigned int unique_id;
	std::string path;
} mapping_t;

typedef struct
{
	std::string base_path;
	std::vector<mapping_t> sub_paths;
} paths_t;

bool load_mappings( const std::string dir_mapping_file, paths_t& p )
{
	std::ifstream ifs(dir_mapping_file);
	if ( !ifs.is_open() )
	{	
		std::cerr << "Unable to open a mapping file : " << dir_mapping_file << std::endl;
		return false;
	}

	std::string buffer;
	std::string unique_id, image_name, vlad_name, tag_name;

	// read mappings
	while (std::getline(ifs, buffer))
	{
		std::istringstream ss( buffer );
		ss >> unique_id >> image_name >> vlad_name >> tag_name;

		if ( unique_id.compare("%basepath:") == 0 ) {
			p.base_path = image_name;
			continue;
		}

		mapping_t m;
		m.unique_id = (unsigned int) atoi( unique_id.c_str() );
		m.path = tag_name;

		p.sub_paths.push_back( m );
	}

	return true;
}

bool load_dataset_settings( const std::string dataset_setting_filename,
						std::string& mapping_file_path,
						std::string& output_path_for_serial_access,
						std::string& output_path_for_random_access)
{
    std::ifstream in(dataset_setting_filename);
    if (!in.is_open()) { std::cerr << "Unable to open a setting file : " << dataset_setting_filename << std::endl; return false; }

    std::string buffer;

    // clear mapping_path string
    mapping_file_path.clear();

    std::string integrated_path;

    // read dataset-setting-file line by line
    while (std::getline(in, buffer))
    {
        std::istringstream iis(buffer);
        std::string key, value;

        iis >> key >> value;


        if (key.compare("datasets:") == 0) break;
        if (key.compare("base:") == 0) integrated_path = value;
        if (key.compare("base_folder:") == 0) integrated_path += value;
        if (key.compare("plain:") == 0) mapping_file_path = integrated_path + value;

        if (key.compare("serial_access:") == 0) output_path_for_serial_access = integrated_path + value;
        if (key.compare("random_access:") == 0) output_path_for_random_access = integrated_path + value;
    }

    return true;
}

bool load_stemmed_tag_file(const std::string path, std::vector<std::string>& tags)
{
    std::ifstream in(path);
    if (!in.is_open()) { std::cerr << "Unable to open a stemmed tag file : " << path << std::endl; return false; }

    // clear tags vector
    tags.clear();

    // read tags line-by-line
    std::string buffer;

    while (std::getline(in, buffer))
    {
        std::istringstream iis(buffer);
        std::string tag;
        iis >> tag;
        tags.push_back(tag);
    }

    return true;
}

/**
  * load terms from a file and return a term vector
  */
void load_terms( std::string term_path, std::vector<std::string>& terms )
{
	std::ifstream ifs;
	ifs.open( term_path.c_str() );
	assert( ifs.is_open() );

	std::string line, term, freq;
	terms.clear();

	while ( std::getline(ifs, line) )
	{
		std::istringstream ss( line );
		ss >> term >> freq;
		terms.push_back( term );
	}

	ifs.close();
}

int main(int argc, char* argv[])
{
	std::string terms_path = "../stemming_tags/statistics/mirflickr1M_unique_tags.txt";

    // check input arguments
    if (argc != 2) {
        std::cerr << "Usage: ./merger [dataset_setting_file]" << std::endl;
        return -1;
    }

    std::string mapping_file_path;
    std::string output_path_for_serial_access;
    std::string output_path_for_random_access;

    // load a yaml dataset setting file
    if (!load_dataset_settings(argv[1], mapping_file_path, output_path_for_serial_access, output_path_for_random_access)) {
        std::cerr << "Error occurs when a yaml dataset setting file is processed." << std::endl;
        return -1;
    }

    paths_t p;

    // load a mapping file
    if (!load_mappings(mapping_file_path, p)) {
        std::cerr << "Error occurs when a mapping file is processed." << std::endl;
        return -1;
    }

	// init inverted index object
	std::vector<std::string> terms;

	InvertedIndex inv_index;

	for (size_t k=0; k<p.sub_paths.size(); k++)
	{
		unsigned int uid = p.sub_paths[k].unique_id;
		if (uid % 1 == 0) std::cout << "Processing " << uid << std::endl;

		std::string tags_path = p.base_path + p.sub_paths[k].path;

		std::vector<std::string> tags;

		if (!load_stemmed_tag_file( tags_path, tags ))
		{
			std::cerr << "Error occurs when a stemmed tag file " << tags_path << " is processed." << std::endl;
			return -1;
		}

		for (size_t m=0; m<tags.size(); m++)
			//inv_index.append_posting( tags[m], uid );
			inv_index.append( tags[m], uid );
	}

	inv_index.save( "tmp/mirflickr1M_invindex.idx" );
	inv_index.close();

	/*
	inv_index.open( "mirflickr1M_invindex.idx");
	std::vector< posting_t > posting = inv_index.get_posting_list( "red" );
	for (int k=0; k<posting.size(); k++) std::cout << "id = " << posting[k] << std::endl;
	inv_index.close();
	*/
}
