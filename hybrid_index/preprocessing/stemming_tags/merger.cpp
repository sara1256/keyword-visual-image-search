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

bool load_mappings(const std::string mapping_filename, paths_t& p)
{
	std::ifstream in(mapping_filename);
	if ( !in.is_open() )
	{
		std::cerr << "Unable to open a mapping file : " << mapping_filename << std::endl;
		return false;
	}

	std::string buffer;
	std::string unique_id, image_name, vlad_name, tag_name;

	// read mappings
	while (std::getline(in, buffer))
	{
		std::istringstream iis(buffer);
		iis >> unique_id >> image_name >> vlad_name >> tag_name;

		if (unique_id.compare("%basepath:") == 0) {
			p.base_path = image_name;
			continue;
		}

		mapping_t m;
		m.unique_id = (unsigned int)atoi(unique_id.c_str());
		m.path = tag_name;

		p.sub_paths.push_back(m);
	}

	return true;
}

// load yaml dataset setting file and get a path of mapping file
bool load_dataset_settings(const std::string dataset_setting_filename,
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

int main(int argc, char* argv[])
{
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

	// make bloom filters
	bloom_filter_manager mgr;

	bloom_parameters param;
	param.projected_element_count = 2500;
	param.false_positive_probability = 1.0 / 1000;
	param.random_seed = 1;
	param.compute_optimal_parameters();

	for (int k=0; k<p.sub_paths.size(); k++)
	{
		unsigned int c = p.sub_paths[k].unique_id;
		if (c % 5000 == 0) std::cout << "Processing " << c << std::endl;

		bloom_filter filter(param);

		std::string tags_path = p.base_path + p.sub_paths[k].path;

		std::vector<std::string> tags;

		if (!load_stemmed_tag_file(tags_path, tags)) {
			std::cerr << "Error occurs when a stemmed tag file " << tags_path << " is processed." << std::endl;
			return -1;
		}

		filter.insert(tags.begin(), tags.end());
		//std::cout << "Filter Size : " << filter.size() / (8 * 1024) << "KB" << std::endl;

		mgr.append(filter);

		//if (k == 0) break;
	}

	mgr.save_for_serial_access(output_path_for_serial_access.c_str());
	mgr.save_for_random_access(output_path_for_random_access.c_str());

	return 0;
}
