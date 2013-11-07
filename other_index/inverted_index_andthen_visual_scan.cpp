#include <cassert>
#include <cmath>
#include <ctime>
#include <iostream>

#include <flann/flann.hpp>
#include <flann/io/hdf5.h>

#include <porter2_stemmer.h>

#include <sys/time.h>

#include <inverted_index.hpp>

#include <indices_lib.hpp>

using namespace flann;

bool compare( std::pair<unsigned int, float> obj1, std::pair<unsigned int, float> obj2)
{
	return obj1.second < obj2.second;
}

int main(int argc, char** argv)
{
	//---------- check input arguments ----------//
	if (argc < 2)
	{
		std::cout << "Usage: ./inverted_index_andthen_visual_scan keyword_1 keyword_2 ... keyword_n" << std::endl;
		return -1;
	}	

	//---------- set data path ----------//
	std::string base_path = "/media/mojool1984/research_storage/mirflickr1M_dataset/working/integrated/";
	std::string vlads_path = base_path + "merged_vlads.hdf5";
	std::string inv_index_path = "../hybrid_index/preprocessing/build_invindex/tmp/mirflickr1M_invindex.idx";

	//---------- load inverted index from file ----------//
	InvertedIndex inv_index;
	inv_index.open( inv_index_path.c_str() );

	//---------- load vlads from file ----------//
	flann::Matrix<float> dataset;

	std::cout << "Loading vlads                      : " << vlads_path << " ... "; std::cout.flush();
	flann::load_from_file(dataset, vlads_path.c_str(), "vladpcas");
	std::cout << "Done." << std::endl; std::cout.flush();

	//---------- processing input keywords ----------//
	std::vector<std::string> terms;
	for (int m=1; m<argc; m++) terms.push_back( Porter2Stemmer::stem( argv[m] ) );

	//---------- set query ----------//
    Matrix<float> query(new float[dataset.cols], 1, dataset.cols);
	for (int c = 0; c < dataset.cols; c++) query[0][c] = dataset[891749][c];

	std::cout << "\ndataset.cols = " << dataset.cols << ",\tdataset.rows = " << dataset.rows << std::endl;
	std::cout << "query.cols = " << query.cols << ",\tquery.rows = " << query.rows << std::endl;

	//---------- record time ----------//
	timeval start2, end2;

	gettimeofday(&start2, 0);

	//---------- firstly do a keyword search ----------//
	std::vector<posting_t> posting = inv_index.find( terms );

	std::vector< std::pair<unsigned int, float> > result;

	//---------- compare vlads ----------//
	for (int m=0; m<posting.size(); m++)
	{
		Matrix<float> vlad(new float[dataset.cols], 1, dataset.cols);

		for (int k=0; k<dataset.cols; k++) vlad[0][k] = dataset[posting[m]][k];

		float dist = calc_distance_of_vectors( query, vlad );
		result.push_back( std::pair<unsigned int, float>( posting[m], dist ) );
	}

	std::cout << "posting size = " << posting.size() << std::endl;

	//---------- record time ----------//
	gettimeofday(&end2, 0);

	std::sort( result.begin(), result.end(), compare );

	int nn = 100;

	std::cout << "\n---------- result ----------\n";
	for (int k=0; k<std::min(int(result.size()), 100); k++) std::cout << result[k].first << " ";
	std::cout << std::endl;

	std::cout << "\n---------- result ----------\n";
	for (int k=0; k<std::min(int(result.size()), 100); k++) std::cout << result[k].second << " ";
	std::cout << std::endl;

	std::cout << "Elapsed msecs!! = " << get_elapsed_time_in_msecs(start2, end2) << std::endl; std::cout.flush();

    return 0;
}
