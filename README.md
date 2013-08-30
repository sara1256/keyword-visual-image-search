keyword-image-index
===================

Developed by Tak-Eun Kim
Korea Advanced Institute of Science and Technology

Index for Content-based Image Search with Keywords


How To Compile
===================

1. run cmake-gui and uncheck c-binding, python, matlab, and mpi
2. cd ./build
3. make flann_cpp
4. move to porter2_stemmer directory
5. make lib (for static library)
6. move to hybrid_index directory
7. run make or run g++ -std=c++0x flann_example.cpp -L/usr/lib -lhdf5 -L../flann/build/lib -lflann_cpp -L../porter2_stemmer -lporter2_stemmer -I../flann/src/cpp -I../porter2_stemmer
