
#include <flann/flann.hpp>
#include <flann/io/hdf5.h>

#include <stdio.h>

using namespace flann;

int main(int argc, char** argv)
{
    int nn = 3;

    Matrix<float> dataset;
    Matrix<float> query;
    Matrix<float> query_tmp;
    load_from_file(dataset, "dataset.hdf5","dataset");
    load_from_file(query, "dataset.hdf5","query");
    //load_from_file(query_tmp, "dataset.hdf5","query");

	/*	
    Matrix<float> query(new float[query_tmp.cols], 1, query_tmp.cols);
	for (int c = 0; c < query_tmp.cols; c++) {
		query[0][c] = query_tmp[0][c];
		std::cout << query[0][c] << std::endl;
	}

	std::cout << "query.cols = " << query.cols << ", query.rows = " << query.rows << std::endl;
	std::cout << "query_tmp.cols = " << query_tmp.cols << ", query_tmp.rows = " << query_tmp.rows << std::endl;
	*/

    Matrix<int> indices(new int[query.rows*nn], query.rows, nn);
    Matrix<float> dists(new float[query.rows*nn], query.rows, nn);

    // construct an randomized kd-tree index using 4 kd-trees
    Index<L2<float> > index(dataset, flann::KDTreeIndexParams(2));
    index.buildIndex();
	index.buildSignature();

    // do a knn search, using 128 checks
    index.knnSearch(query, indices, dists, nn, flann::SearchParams(128));

    flann::save_to_file(indices,"result.hdf5","result");

    delete[] dataset.ptr();
    delete[] query.ptr();
    delete[] indices.ptr();
    delete[] dists.ptr();
    
    return 0;
}
