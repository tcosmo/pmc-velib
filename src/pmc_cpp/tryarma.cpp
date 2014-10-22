#include "armadillo"
#include <iostream>

using namespace std;
using namespace arma;

int main()
{
    srand(time(NULL));
    mat A = zeros<mat>(5,5);
    A.imbue([]() { 
            float a = -0.5, b = 0.5;
            float random = ((float) rand()) / (float) RAND_MAX;
            float diff = b - a;
            float r = random * diff;
            return a+r;});
    cout << A;

}
