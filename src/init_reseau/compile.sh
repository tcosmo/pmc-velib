g++ -o init_reseau main.cpp -I/usr/include ../pmc_cpp/tinystr.cpp ../pmc_cpp/tinyxml.cpp ../pmc_cpp/tinyxmlerror.cpp ../pmc_cpp/tinyxmlparser.cpp -std=c++0x
cp init_reseau ../../
rm init_reseau
