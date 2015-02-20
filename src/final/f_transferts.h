#include <cmath>

double sigmoide(double t)
{
    return 1/(exp(-1*t)+1);
}

double sigmoder(double t)
{
    return exp(-1*t)/(pow((exp(-1*t)+1),2));
}


double lin(double t)
{
    return t;
}

double linder(double t)
{
    return 1;
}

