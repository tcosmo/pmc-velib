#include <cmath>

double sigmoide(double n)
{
    return 1/(exp(-1*n)+1);
}

double sigmoder(double n)
{
    return exp(-1*n)/(pow((exp(-1*n)+1),2));
}

