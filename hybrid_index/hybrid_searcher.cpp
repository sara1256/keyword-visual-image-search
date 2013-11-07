#include <cassert>
#include <iostream>

#include <flann/flann.hpp>
#include <flann/io/hdf5.h>

#include <porter2_stemmer.h>

#include <sys/time.h>

#include <indices_lib.hpp>

using namespace flann;

int main(int argc, char** argv)
{
	//---------- set data path ----------//
	std::string base_path = "/media/mojool1984/research_storage/mirflickr1M_dataset/working/integrated/";
	std::string vlads_path = base_path + "merged_vlads.hdf5";
	//std::string leaf_signature_path = base_path + "merged_tags_for_random_access.dat";
	std::string leaf_signature_path = "merged_tags_for_random_access.dat";
	std::string nonleaf_signature_path = "nonleaf_ok.dat";

	//---------- load vlads from file ----------//
	flann::Matrix<float> dataset;

	std::cout << "Loading vlads                      : " << vlads_path << " ... "; std::cout.flush();
	flann::load_from_file(dataset, vlads_path.c_str(), "vladpcas");
	std::cout << "Done." << std::endl; std::cout.flush();

	Index<L2<float> > index(dataset, flann::SavedIndexParams("index.idx"), leaf_signature_path, nonleaf_signature_path);

    int nn = 100;

    Matrix<float> query_tmp;
    load_from_file(query_tmp, "dataset.hdf5","query");

    Matrix<float> query(new float[query_tmp.cols], 1, query_tmp.cols);
	for (int c = 0; c < query_tmp.cols; c++) {
		//query[0][c] = query_tmp[0][c];
		//query[0][c] = dataset[290137][c];
		query[0][c] = dataset[891749][c];
		//std::cout << query[0][c] << " ";
	}
	std::cout << std::endl;

	std::cout << "\ndataset.cols = " << dataset.cols << ", dataset.rows = " << dataset.rows << std::endl;
	std::cout << "\nquery.cols = " << query.cols << ", query.rows = " << query.rows << std::endl;

    Matrix<int> indices(new int[query.rows*nn], query.rows, nn);
    Matrix<float> dists(new float[query.rows*nn], query.rows, nn);

	// do a knn search with keyword query
	std::vector<std::string> keywords;

	if (argc <= 1)
	{
		std::cout << "Usage: ./hybrid_searcher keyword1 keyword2 ..." << std::endl;
		return 0;
	}
	
	for (int k=1; k<argc; k++) keywords.push_back( Porter2Stemmer::stem( argv[k] ) );

	timeval start, end;

	gettimeofday(&start, 0);

	//int num_results = index.knnSearch2(query, keywords, indices, dists, nn, flann::SearchParams(256));
	int num_results = index.knnSearch2(query, keywords, indices, dists, nn, flann::SearchParams(10));

	gettimeofday(&end, 0);

	std::cout << "num_results = " << num_results << std::endl;

	std::cout << "\n---------- result ----------\n";
	display_result( indices, num_results );

	std::cout << "\n---------- result ----------\n";
	display_result( dists, num_results );

	std::cout << "\nElapsed msecs = " << get_elapsed_time_in_msecs(start, end) << std::endl;

    delete[] dataset.ptr();
    delete[] query.ptr();
    delete[] indices.ptr();
    delete[] dists.ptr();

    return 0;
}
