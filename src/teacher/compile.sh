g++ -o teacher main.cpp  -I /Users/tristansterin/Documents/Cours/TIPE/pmc-velib/lib/armadillo-4.300.3/include ../../lib/tinyxml/tinystr.cpp ../../lib/tinyxml/tinyxml.cpp ../../lib/tinyxml/tinyxmlerror.cpp ../../lib/tinyxml/tinyxmlparser.cpp -std=c++0x -O1 -framework Accelerate -DRAMA_USE_BLAS -DRAMA_USE_LAPACK -lblas -llapack
cp teacher ../../
rm teacher
