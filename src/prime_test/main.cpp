#include <cstdio>
#include "../pmc_cpp/PMC.h"

using namespace std;

int main()
{
    vector<tuple<mat,mat>> base;
    int nb;
    FILE* f = fopen("primes","r");
    fscanf(f,"%d", &nb);
    for(int i = 0 ; i < nb ; i++)
    {
        int prime;
        fscanf(f,"%d", &prime);
        double i_norm = double(i*2)/(nb-1)-1;
        double p_norm = double(prime*0.90)/1000+0.05;
        vector<double> a = {i_norm};
        vector<double> b = {p_norm};
        base.push_back(make_tuple(colvec(a),colvec(b)));
    }
    /*for(int i = 0 ; i < base.size() ; i++)
    {
        printf("%lf, %lf\n", get<0>(base[i])(0,0), get<1>(base[i])(0,0));
    }*/

    fclose(f);

    Reseau r(1);
    Couche c1;
    
    for(int i = 0 ; i < 5 ; i++)
        c1.neurones.push_back(Neurone(sigmoide,sigmoder));

    Couche sortie;
    sortie.neurones.push_back(Neurone(sigmoide,sigmoder));
    r.couches.push_back(c1);
    r.couches.push_back(sortie);

    PMC pmc(r, 8, base);
    for(int i = 0 ; i < 50000 ; i++)
        pmc.cycle_apprentissage();

    printf("DONE\n");
    
    while(1)
    {   
        int i;
        scanf("%d",&i);
        double i_norm = double(i*2)/(nb-1)-1;
        vector<double> a = {i_norm};
        pmc.reseau.calcule_sortie(a);
        double reponse = (pmc.reseau.sorties[r.couches.size()-1])(0,0);
        
        printf("%lf\n", (reponse-0.05)*1000/(0.90));
    }
}
