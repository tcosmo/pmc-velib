#include <iostream>

#include "PMC.h"

int main()
{
    mat A = colvec({-0.4,0.6,-0.87});
    mat B = colvec({0.09,0.56});

    vector<tuple<mat,mat>> base = {make_tuple(A,B)};
    
    Reseau r(3);

    Couche C1;
    for(int i = 0 ; i < 4 ; i++)
        C1.neurones.push_back(Neurone(sigmoide,sigmoder));

    Couche C2;
    for(int i = 0 ; i < 3 ; i++)
        C2.neurones.push_back(Neurone(sigmoide,sigmoder));
    
    Couche C3;
    for(int i = 0 ; i < 2 ; i++)
        C3.neurones.push_back(Neurone(sigmoide,sigmoder));

    r.couches.push_back(C1);
    r.couches.push_back(C2);
    r.couches.push_back(C3);
    
    
    PMC pmc(r, 10, base);
    
    for(int i = 0 ; i < 10 ; i++)
        pmc.cycle_apprentissage(); 
    cout << pmc.reseau.couches[pmc.reseau.couches.size()-1].sortie;

    //for(int i = -1 ; i < 100 ; i++)
      //  pmc.cycle_apprentissage();

    return 0;
}
