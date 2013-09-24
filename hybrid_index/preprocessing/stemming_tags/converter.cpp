/**
 * @author Tak-Eun Kim
 * @date September 2013
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>

#include <keyword/bloom_filter_manager.hpp>

namespace bfs = boost::filesystem;

// load yaml dataset setting file and get a path of mapping file
bool load_dataset_settings(const std::string dataset_setting_filename,
						std::string& serialized_bloom_filter_file_path,
						std::string& random_accessible_bloom_filter_file_path)
{
	std::ifstream in(dataset_setting_filename);
	if (!in.is_open()) { std::cerr << "Unable to open a setting file : " << dataset_setting_filename << std::endl; return false; }

	std::string buffer;

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
		if (key.compare("serial_access:") == 0) serialized_bloom_filter_file_path = integrated_path + value;
		if (key.compare("random_access:") == 0) random_accessible_bloom_filter_file_path = integrated_path + value;
	}

	return true;
}

int main(int argc, char* argv[])
{
	// check input arguments
	if (argc != 2) {
		std::cerr << "Usage: ./converter [dataset_setting_file]" << std::endl;
		return -1;
	}

	std::string serialized_bloom_filter_file_path;
	std::string random_accessible_bloom_filter_file_path;

	// load a yaml dataset setting file
	if (!load_dataset_settings(argv[1], serialized_bloom_filter_file_path, random_accessible_bloom_filter_file_path)) {
		std::cerr << "Error occurs when a yaml dataset setting file is processed." << std::endl;
		return -1;
	}

	bloom_filter_manager mgr_for_serial_access;
	bloom_filter_manager mgr_for_random_access;

	std::cout << "Loading serialized bloom filter data file: " << serialized_bloom_filter_file_path << " ..." << std::flush;
	mgr_for_serial_access.load_for_serial_access( serialized_bloom_filter_file_path.c_str() );
	std::cout << "Done." << std::endl << std::flush;

	for (int k=0; k<mgr_for_serial_access.size(); k++)
	{
		bloom_filter filter = mgr_for_serial_access[k];

		mgr_for_random_access.append( filter );

		if (k % 5000 == 0) std::cout << "Processing " << k << std::endl;
	}

	std::cout << "Saving random accessible bloom filters to the file: " << random_accessible_bloom_filter_file_path << " ..." << std::flush;
	mgr_for_random_access.save_for_random_access( random_accessible_bloom_filter_file_path.c_str() );
	std::cout << "Done." << std::endl << std::flush;

	mgr_for_random_access.close_for_random_access();

	return 0;
}
