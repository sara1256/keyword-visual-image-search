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
4. move to an example directory
5. g++ flann_example.cpp -L/usr/lib -lhdf5 -L../build/lib -lflann_cpp -I../src/cpp
