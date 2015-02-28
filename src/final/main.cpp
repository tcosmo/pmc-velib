#include <cstdio>
#include "modele.h"

/* Attention : collusion malheureuse entre std::cout (printf c++) et modele.cout */

void print_vec(vector<double> v)
{
    for(double a: v)
        printf("%lf\n", a);
}

void exemple_carre()
{
    /* Exemple apprentissage de la focntion carré sur [-1,1] à partir de 1500 exemples */

    vector<t_exemple> ens;
    double range = 1; // Augmenter le range au prix d'une complexité plus élevée, en pratique
                      // on normalisera les sorties dans [-1,1] 
    
    srand(time(NULL)); // Graine sur temps actuel
    
    for(int i = 0 ; i < 1500 ; i++)
    {
        double x = rand_float(-range,range);
        ens.push_back(make_pair(vector<double>({x,1}),x*x));
    }
    
    modele g(2,5);
    std::cout.precision(20);
    std::cout << "Cout initial : " << g.cout(ens) << endl;
    g.optimise(100, ens);
    std::cout << "Cout final : " << g.cout(ens) << endl;
}

int main()
{
    exemple_carre();
}
