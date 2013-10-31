#include <cassert>
#include <cmath>

#include <sys/time.h>
#include <flann/flann.hpp>

using namespace flann;

double get_elapsed_time_in_msecs( const timeval begin_time, const timeval end_time )
{
	long elapsed_sec = end_time.tv_sec - begin_time.tv_sec;
	long elapsed_usec = end_time.tv_usec - begin_time.tv_usec;

	return elapsed_sec * 1000.0 + elapsed_usec / 1000.0;
}

/*
 *	both v1 & v2 are row vectors
 */
float calc_distance_of_vectors( const Matrix<float> v1, const Matrix<float> v2 )
{
	assert( v1.cols == v2.cols );
	assert( v1.rows == 1 );
	assert( v2.rows == 1 );

	float dist = 0.0;
	for (int k=0; k<v1.cols; k++) dist += std::pow( v1[0][k] - v2[0][k], 2 );
	return std::sqrt( dist );
}

template <typename T>
void display_result( const Matrix<T> in, unsigned int num_results )
{
	unsigned int cnt = 0;

	for (int r=0; r<in.rows; r++) {
		std::cout << "[" << r << "] : ";
		for (int c=0; c<in.cols; c++) {
			if (cnt++ >= num_results) break;
			std::cout << in[r][c] << " ";
		}
		std::cout << std::endl;
	}
}
