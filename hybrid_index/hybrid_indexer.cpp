#include <flann/flann.hpp>
#include <flann/io/hdf5.h>

#include <iostream>

int main(int argc, char* argv[])
{
	//---------- set data path ----------//
	std::string base_path = "/media/mojool1984/research_storage/mirflickr1M_dataset/working/integrated/";
	std::string vlads_path = base_path + "merged_vlads.hdf5";
	std::string tags_path = base_path + "merged_tags_for_serial_access.dat";

	//---------- load vlads from file ----------//
	flann::Matrix<float> dataset;

	std::cout << "Loading vlads       : " << vlads_path << " ... "; std::cout.flush();
	flann::load_from_file(dataset, vlads_path.c_str(), "vladpcas");
	std::cout << "Done." << std::endl; std::cout.flush();

	//---------- load tags signatures from file ----------//
	bloom_filter_manager signatures;

	std::cout << "Loading signatures  : " << tags_path << " ... "; std::cout.flush();
	signatures.load_for_serial_access( tags_path.c_str() );
	std::cout << "Done." << std::endl; std::cout.flush();

	std::cout << "\ndataset.cols = " << dataset.cols << ", dataset.rows = " << dataset.rows << std::endl;

	//---------- construct 4 randomized kd-trees ----------//
    flann::Index<flann::L2<float> > index(dataset, flann::KDTreeIndexParams(4));

	//---------- build 4 randomized kd-trees ----------//
	std::cout << "Building kd-trees ... "; std::cout.flush();
    index.buildIndex();
	std::cout << "Done." << std::endl; std::cout.flush();

	//---------- build signatures for each kd-tree ----------//
	std::cout << "Building signatures ... "; std::cout.flush();
	index.buildSignature(signatures);
	std::cout << "Done." << std::endl; std::cout.flush();

	//---------- save indices ----------//
	std::cout << "Saving kd-tree index ... "; std::cout.flush();
	index.save("index.idx");
	std::cout << "Done." << std::endl; std::cout.flush();

	std::cout << "Saving signature index ... "; std::cout.flush();
	index.saveSignatureIndex("nonleaf_ok.dat");
	std::cout << "Done." << std::endl; std::cout.flush();

    delete[] dataset.ptr();

	return 0;
}
