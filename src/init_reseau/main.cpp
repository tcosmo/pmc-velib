#include <cstdio>
#include "../pmc_cpp/Neurone.h"

int main(int argc, char *argv[])
{
/*
    printf("%d\n", argc);
    for(int i = 0 ; i < argc ; i++)
        printf("%s\n", argv[i]);
*/
    int format_entree = atoi(argv[2]);
//  printf("%d\n", format_entree);

    Reseau r(format_entree);
    double etha = atof(argv[3]);
    int vrai_nb_couche = atoi(argv[4])-2;
    
    for(int i = 0 ; i < vrai_nb_couche ; i++)
    {
        Couche c;
        int nb_neurones = atoi(argv[5+i]);
        for(int j = 0 ; j < nb_neurones ; j++)
            c.neurones.push_back(Neurone(sigmoide,sigmoder));
        r.couches.push_back(c);
    } 

    Couche sortie;
    for(int j = 0 ; j < 2 ; j++)
        sortie.neurones.push_back(Neurone(sigmoide,sigmoder));
    
    r.couches.push_back(sortie);
    r.init_reseau();
    r.save_reseau(string(argv[1])+"network", etha);

}
