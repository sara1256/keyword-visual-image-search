
#include <flann/flann.hpp>
#include <flann/io/hdf5.h>

#include <stdio.h>

#include <porter2_stemmer.h>

using namespace flann;

int main(int argc, char** argv)
{
    int nn = 100;

    Matrix<float> dataset;
    //Matrix<float> query;
    Matrix<float> query_tmp;
    //load_from_file(dataset, "dataset.hdf5","dataset");
	load_from_file(dataset, "/media/mojool1984/research_storage/mirflickr1M_dataset/working/integrated/merged_vlads.hdf5", "vladpcas");
    //load_from_file(query, "dataset.hdf5","query");
    load_from_file(query_tmp, "dataset.hdf5","query");

    Matrix<float> query(new float[query_tmp.cols], 1, query_tmp.cols);
	for (int c = 0; c < query_tmp.cols; c++) {
		query[0][c] = query_tmp[0][c];
		std::cout << query[0][c] << " ";
		//dataset[0][c] = dataset[0][c];
		//std::cout << dataset[0][c] << " ";
	}
	std::cout << std::endl;

	std::cout << "\ndataset.cols = " << dataset.cols << ", dataset.rows = " << dataset.rows << std::endl;
	std::cout << "\nquery.cols = " << query.cols << ", query.rows = " << query.rows << std::endl;

    Matrix<int> indices(new int[query.rows*nn], query.rows, nn);
    Matrix<float> dists(new float[query.rows*nn], query.rows, nn);

    // construct an randomized kd-tree index using 4 kd-trees
    //Index<L2<float> > index(dataset, flann::KDTreeIndexParams(4));
    //index.buildIndex();
	//index.buildSignature();
	//index.save("index.idx");

	std::cout << "Indexing is done." << std::endl;

	Index<L2<float> > index(dataset, flann::SavedIndexParams("index.idx"));
	//index.buildSignature();

    // do a knn search, using 128 checks
    int count = index.knnSearch(query, indices, dists, nn, flann::SearchParams(128));
	std::cout << "count = " << count << std::endl;

	// do a knn search with keyword query
	std::vector<std::string> keywords;
	keywords.push_back("apple");
	keywords.push_back("lemon");

	std::cout << "run knnSearch..." << std::endl;
	//int count2 = index.knnSearch2(query, keywords, indices, dists, nn, flann::SearchParams(128));
	int count2 = index.knnSearch(query, indices, dists, nn, flann::SearchParams(128));
	std::cout << "count2 = " << count2 << std::endl;

	for (int r=0; r<indices.rows; r++) {
		for (int c=0; c<indices.cols; c++) {
			std::cout << indices[r][c] << " ";
		}
		std::cout << std::endl;
	}

    //flann::save_to_file(indices,"result.hdf5","result");

    delete[] dataset.ptr();
    delete[] query.ptr();
    delete[] indices.ptr();
    delete[] dists.ptr();

	for (int k=0; k<keywords.size(); k++)
		std::cout << Porter2Stemmer::stem(keywords[k]) << std::endl;
    
    return 0;
}
