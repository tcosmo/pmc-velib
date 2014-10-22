g++ -o teacher main.cpp  -I /Users/tristansterin/Documents/Cours/TIPE/pmc-velib/lib/armadillo-4.300.3/include ../pmc_cpp/tinystr.cpp ../pmc_cpp/tinyxml.cpp ../pmc_cpp/tinyxmlerror.cpp ../pmc_cpp/tinyxmlparser.cpp -std=c++0x -O1 -framework Accelerate -DRAMA_USE_BLAS -DRAMA_USE_LAPACK -lblas -llapack
cp teacher ../../
rm teacher
