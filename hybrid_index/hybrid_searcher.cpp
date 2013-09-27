#include <cassert>
#include <ctime>
#include <iostream>

#include <flann/flann.hpp>
#include <flann/io/hdf5.h>

#include <porter2_stemmer.h>

using namespace flann;

int main(int argc, char** argv)
{
	//---------- set data path ----------//
	std::string base_path = "/media/mojool1984/research_storage/mirflickr1M_dataset/working/integrated/";
	std::string vlads_path = base_path + "merged_vlads.hdf5";
	std::string leaf_signature_path = base_path + "merged_tags_for_random_access.dat";
	std::string nonleaf_signature_path = "nonleaf_ok.dat";

	//---------- load vlads from file ----------//
	flann::Matrix<float> dataset;

	std::cout << "Loading vlads                      : " << vlads_path << " ... "; std::cout.flush();
	flann::load_from_file(dataset, vlads_path.c_str(), "vladpcas");
	std::cout << "Done." << std::endl; std::cout.flush();



	Index<L2<float> > index(dataset, flann::SavedIndexParams("index.idx"), leaf_signature_path, nonleaf_signature_path);



    int nn = 100;

    Matrix<float> query_tmp;
    //load_from_file(query, "dataset.hdf5","query");
    load_from_file(query_tmp, "dataset.hdf5","query");

    Matrix<float> query(new float[query_tmp.cols], 1, query_tmp.cols);
	for (int c = 0; c < query_tmp.cols; c++) {
		//query[0][c] = query_tmp[0][c];
		//query[0][c] = dataset[290137][c];
		query[0][c] = dataset[891749][c];
		std::cout << query[0][c] << " ";
	}
	std::cout << std::endl;

	std::cout << "\ndataset.cols = " << dataset.cols << ", dataset.rows = " << dataset.rows << std::endl;
	std::cout << "\nquery.cols = " << query.cols << ", query.rows = " << query.rows << std::endl;

    Matrix<int> indices(new int[query.rows*nn], query.rows, nn);
    Matrix<float> dists(new float[query.rows*nn], query.rows, nn);



	// do a knn search with keyword query
	std::vector<std::string> keywords;
	//keywords.push_back("cigarett");
	//keywords.push_back("tattoo");
	//keywords.push_back("smoke");
	//keywords.push_back( "sanfrancisco" );
	keywords.push_back( "berlin" );

	clock_t begin = clock();	

	// TODO
	//  - run porter stemmer here
	std::vector<std::string> stemmed_keywords;

	for (int k=0; k<keywords.size(); k++)
		stemmed_keywords.push_back( Porter2Stemmer::stem( keywords[k] ) );

	int count2 = index.knnSearch2(query, stemmed_keywords, indices, dists, nn, flann::SearchParams(256));

	clock_t end = clock();

	std::cout << "count2 = " << count2 << std::endl;

	std::cout << "\n---------- result ----------\n";
	for (int r=0; r<indices.rows; r++) {
		for (int c=0; c<indices.cols; c++) {
			std::cout << indices[r][c] << " ";
		}
		std::cout << std::endl;
	}

	std::cout << "\n---------- result ----------\n";
	for (int r=0; r<indices.rows; r++) {
		for (int c=0; c<indices.cols; c++) {
			std::cout << dists[r][c] << " ";
		}
		std::cout << std::endl;
	}

	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

	std::cout << "Elapsed secs = " << elapsed_secs << std::endl;

    //flann::save_to_file(indices,"result.hdf5","result");
	count2 = index.knnSearch(query, indices, dists, nn, flann::SearchParams(256));
	std::cout << "count2 = " << count2 << std::endl;

	std::cout << "\n---------- result ----------\n";
	for (int r=0; r<indices.rows; r++) {
		for (int c=0; c<indices.cols; c++) {
			std::cout << indices[r][c] << " ";
		}
		std::cout << std::endl;
	}

	std::cout << "\n---------- result ----------\n";
	for (int r=0; r<indices.rows; r++) {
		for (int c=0; c<indices.cols; c++) {
			std::cout << dists[r][c] << " ";
		}
		std::cout << std::endl;
	}

    delete[] dataset.ptr();
    delete[] query.ptr();
    delete[] indices.ptr();
    delete[] dists.ptr();

	for (int k=0; k<keywords.size(); k++)
		std::cout << Porter2Stemmer::stem(keywords[k]) << std::endl;
    
    return 0;
}
