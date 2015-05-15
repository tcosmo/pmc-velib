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

vector<t_exemple> creer_set()
{
    vector<t_exemple> toReturn;
    FILE *f = fopen("norm.data", "r");
    int nb;
    fscanf(f,"%d\n", &nb);
    printf("%d\n", nb);
    for(int i = 0 ; i < nb ; i++)
    {
        double h,m,t,p,s,inutile;
        vector<double> entree;
        
        fscanf(f,"%lf %lf %lf %lf %lf %lf\n", &h, &m, &t, &p, &s, &inutile);
        entree = vector<double>{h,m,t,p,1};
        toReturn.push_back(make_pair(entree,s));
    }
    fclose(f);
    return toReturn;
}

void save_modele(modele g)
{
    FILE *f = fopen("modele.data", "w");
    fprintf(f, "%d %d\n", g.nb_entrees, g.complexite);
    for(int i = 0 ; i < g.poids.size() ; i++)
        for(int j = 0 ; j < g.poids[i].size() ; j++)
            fprintf(f, "%lf\n", g.poids[i][j]);
    fclose(f);
}

int main()
{
    //exemple_carre();
    auto ens = creer_set();
    modele g(5,15);
    std::cout.precision(20);
    std::cout << "Cout initial : " << g.cout(ens) << endl;
    g.optimise(500, ens);
    std::cout << "Cout final : " << g.cout(ens) << endl;
    save_modele(g);
}
