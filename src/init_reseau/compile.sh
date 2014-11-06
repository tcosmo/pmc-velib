g++ -o init_reseau main.cpp -I/usr/include ../../lib/tinyxml/tinystr.cpp ../../lib/tinyxml/tinyxml.cpp ../../lib/tinyxml/tinyxmlerror.cpp ../../lib/tinyxml/tinyxmlparser.cpp -std=c++0x
cp init_reseau ../../
rm init_reseau
