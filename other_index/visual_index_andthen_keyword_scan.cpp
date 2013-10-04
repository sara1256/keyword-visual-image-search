#include <cassert>
#include <ctime>
#include <iostream>

#include <flann/flann.hpp>
#include <flann/io/hdf5.h>

#include <porter2_stemmer.h>

#include <sys/time.h>

#include <keyword/bloom_filter_manager.hpp>

using namespace flann;

int main(int argc, char** argv)
{
	//---------- set data path ----------//
	std::string base_path = "/media/mojool1984/research_storage/mirflickr1M_dataset/working/integrated/";
	std::string vlads_path = base_path + "merged_vlads.hdf5";
	std::string signature_path = base_path + "merged_tags_for_random_access.dat";
	std::string nonleaf_signature_path = "../hybrid_index/nonleaf_ok.dat";

	//---------- load vlads from file ----------//
	flann::Matrix<float> dataset;

	std::cout << "Loading vlads                      : " << vlads_path << " ... "; std::cout.flush();
	flann::load_from_file(dataset, vlads_path.c_str(), "vladpcas");
	std::cout << "Done." << std::endl; std::cout.flush();



	Index<L2<float> > index( dataset, flann::SavedIndexParams("../hybrid_index/index.idx") );



    int nn = 600000;

    Matrix<float> query_tmp;
    //load_from_file(query, "dataset.hdf5","query");
    load_from_file(query_tmp, "../hybrid_index/dataset.hdf5","query");

    Matrix<float> query(new float[query_tmp.cols], 1, query_tmp.cols);
	for (int c = 0; c < query_tmp.cols; c++) {
		query[0][c] = dataset[891749][c];
		std::cout << query[0][c] << " ";
	}
	std::cout << std::endl;

	std::cout << "\ndataset.cols = " << dataset.cols << ", dataset.rows = " << dataset.rows << std::endl;
	std::cout << "\nquery.cols = " << query.cols << ", query.rows = " << query.rows << std::endl;

    Matrix<int> indices(new int[query.rows*nn], query.rows, nn);
    Matrix<float> dists(new float[query.rows*nn], query.rows, nn);



	std::vector<std::string> keywords;

	if (argc <= 1)
	{
		std::cout << "Usage: ./hybrid_searcher keyword1 keyword2 ..." << std::endl;
		return 0;
	}
	
	for (int k=1; k<argc; k++) keywords.push_back( argv[k] );

	std::vector<std::string> stemmed_keywords;

	for (int k=0; k<keywords.size(); k++)
		stemmed_keywords.push_back( Porter2Stemmer::stem( keywords[k] ) );


	// open bloom filters
	bloom_filter_manager mgr;

	assert( mgr.open_for_random_access( signature_path.c_str() ) );
	mgr.close_for_random_access();

	timeval start2, end2;

	gettimeofday(&start2, 0);

	//int count2 = index.knnSearch(query, indices, dists, nn, flann::SearchParams(FLANN_CHECKS_UNLIMITED));
	int count2 = index.knnSearch(query, indices, dists, nn, flann::SearchParams(256));
	std::cout << "count2 = " << count2 << std::endl;

	/*
	std::cout << "\n---------- result ----------\n";
	for (int c=0; c<indices.cols; c++) std::cout << indices[0][c] << " ";
	std::cout << std::endl;
	*/

	std::cout << "\n---------- result ----------\n";
	for (int c=0; c<indices.cols; c++) {
		bloom_filter *filter = mgr.load_for_random_access( indices[0][c] );
		std::vector<std::string>::iterator iter = filter->contains_all( stemmed_keywords.begin(), stemmed_keywords.end() );
		if (stemmed_keywords.end() == iter) std::cout << indices[0][c] << " ";
		//else std::cout << " - ";
	}
	std::cout << std::endl;

	gettimeofday(&end2, 0);

	long seconds2 = end2.tv_sec - start2.tv_sec;
	long useconds2 = end2.tv_usec - start2.tv_usec;
	std::cout << "Elapsed msecs = " << (seconds2 * 1000.0 + useconds2/1000.0) << std::endl; std::cout.flush();



	std::cout << "\n---------- result ----------\n";
	for (int c=0; c<indices.cols; c++) std::cout << dists[0][c] << " "; std::cout << std::endl;

    delete[] dataset.ptr();
    delete[] query.ptr();
    delete[] indices.ptr();
    delete[] dists.ptr();
    
    return 0;
}
