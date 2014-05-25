#include <cmath>
#include "Neurone.h"

double sigmoide(double n)
{
    return 1/(exp(-1*n)+1);
}

double sigmoder(double n)
{
    return exp(-1*n)/(pow((exp(-1*n)+1),2));
}

class PMC
{
    public:
        
        PMC(Reseau p_reseau, double p_etha, vector<tuple<mat,mat>> p_base_appretissage = vector<tuple<mat,mat>>())
        {
            reseau = p_reseau;
            etha = p_etha;
            base_appretissage = p_base_appretissage;
            sensibilite = vector<mat>();
            init_pmc();
        }
        
        void init_pmc()
        {
            reseau.init_reseau();
        }

        mat calcule_matrice_der(int k)
        {
            vector<double> val_der;
            for(Neurone n: reseau.couches[k].neurones)
                val_der.push_back(sigmoder(n.activation(0,0)));
            rowvec c(val_der);
            return diagmat(c);
        }
   
        void calcule_sensibilite(tuple<mat,mat> couple_final)
        {
            sensibilite.clear();
            auto der = calcule_matrice_der(reseau.couches.size()-1);
            sensibilite.push_back(-2*der*(get<1>(couple_final)-reseau.sorties[reseau.couches.size()-1]));
            
            int k = reseau.couches.size()-2;
            while(k >= 1)
            {
                der = calcule_matrice_der(k);
                auto poids = reseau.couches[k+1].calcule_mat_poids();
                sensibilite.push_back(der*(poids)*sensibilite[sensibilite.size()-1]);
                
                k-=1;
            }

            reverse(sensibilite.begin(),sensibilite.end());
        }

        void cycle_apprentissage(bool last = false)
        {
            random_shuffle(base_appretissage.begin(), base_appretissage.end());
            
            for(auto couple: base_appretissage)
            {
                reseau.calcule_sortie(get<0>(couple));
                calcule_sensibilite(couple);
                
                for(int k = 0 ; k < reseau.couches.size() ; k++)
                    for(int i = 0 ; i < reseau.couches[k].neurones.size() ; i++)
                    {
                        mat prod = sensibilite[k-1]*reseau.sorties[k-1].t();
                        auto ajout = (-1*etha*(prod.row(i)));
                        reseau.couches[k].neurones[i].poids += ajout.t();
                        reseau.couches[k].neurones[i].biais += etha*sensibilite[k-1][i];
                    }
            }
        }

        Reseau reseau;
        double etha;
        vector<tuple<mat,mat>> base_appretissage;
        vector<mat> sensibilite;
        
};
