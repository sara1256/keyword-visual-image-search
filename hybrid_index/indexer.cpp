#include <flann/flann.hpp>
#include <flann/io/hdf5.h>

#include <iostream>

using namespace flann;

int main(int argc, char* argv[])
{
	Matrix<float> dataset;
	load_from_file(dataset, "/media/mojool1984/research_storage/mirflickr1M_dataset/working/integrated/merged_vlads.hdf5", "vladpcas");

	std::cout << "\ndataset.cols = " << dataset.cols << ", dataset.rows = " << dataset.rows << std::endl;

	/*
	Matrix<float> dataset2(new float[dataset.cols*2], 2, dataset.cols);
    Index<L2<float> > index(dataset2, flann::KDTreeIndexParams(1));
	for (int r=0; r<2; r++)
	{
		for (int c=0; c < dataset.cols; c++) {
			dataset2[r][c] = dataset[r][c];
			std::cout << dataset2[r][c] << " ";
		}
		std::cout << std::endl;
	}
	*/

    // construct an randomized kd-tree index using 4 kd-trees
    //Index<L2<float> > index(dataset, flann::KDTreeIndexParams(4));
    //index.buildIndex();
	Index<L2<float> > index(dataset, flann::SavedIndexParams("index.idx"));
	index.buildSignature();
	//index.save("index.idx");

	std::cout << "Indexing is done." << std::endl;

    delete[] dataset.ptr();

	return 0;
}
