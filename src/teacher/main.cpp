#include <cstdio>
#include <cstdlib>

#include "../pmc_cpp/PMC.h"

using namespace std;

//Bases d'apprentissages : entrainement et validation
vector<tuple<mat,mat>> base_app;
vector<tuple<mat,mat>> base_val;

void remplit_base(string path, int format_entree)
{
    freopen((path+"norm.data").c_str(),"r", stdin);
    
    int nb_lignes_app;
    scanf("%d", &nb_lignes_app);

    for(int iLig = 0 ; iLig < nb_lignes_app ; iLig++)
    {
        vector<double> entree;
        for(int ie = 0 ; ie < format_entree ; ie++)
        {
            double e;
            scanf("%lf", &e);
            entree.push_back(e);
        }
        vector<double> sortie;
        double s1, s2;
        scanf("%lf %lf", &s1, &s2);
        sortie.push_back(s1);
        sortie.push_back(s2);
        base_app.push_back(make_tuple(colvec(entree),colvec(sortie)));
    }
    
    int nb_lignes_val;
    scanf("%d", &nb_lignes_val);
    for(int iLig = 0 ; iLig < nb_lignes_val ; iLig++)
    {
        vector<double> entree;
        for(int ie = 0 ; ie < format_entree ; ie++)
        {
            double e;
            scanf("%lf", &e);
            entree.push_back(e);
        }
        vector<double> sortie;
        double s1, s2;
        scanf("%lf %lf", &s1, &s2);
        sortie.push_back(s1);
        sortie.push_back(s2);
        base_val.push_back(make_tuple(colvec(entree),colvec(sortie)));
    }

}


int main(int argc, char *argv[])
{
    string path = string(argv[1])+"/";
    Reseau r(path+"network.xml");
    remplit_base(path, r.format_entree);    
    PMC pmc(r, r.etha_read, base_app);

    int curr_ite = 0;
    double erreur_sur_valid = 0.0;
    double min_erreur_valid = 1000*1000*1000;

    FILE* f = fopen((path+"learning.log").c_str(),"w");

    int nb_fois = 0;
    int nb_ite_max = atoi(argv[argc-1]); 
    while(1)
    {
        if(curr_ite % 10000 == 0)
        {
            printf("Iteration %d/%d\n", curr_ite, nb_ite_max);
            if(nb_fois > nb_ite_max/10000-1)
                break;
            fprintf(f, "Iteration %d\n", curr_ite);
            nb_fois++;
        }

        pmc.cycle_apprentissage();
        erreur_sur_valid = pmc.reseau.calcule_erreur_moyq(base_val);
        
        if(erreur_sur_valid < min_erreur_valid)
        {
            double erreur_sur_app = pmc.reseau.calcule_erreur_moyq(base_app);
            printf("Amelioration : %d; Erreur validation : %lf; Erreur apprentissage : %lf\n", curr_ite, erreur_sur_valid, erreur_sur_app);
            fprintf(f,"Amelioration : %d; Erreur validation : %lf; Erreur apprentissage : %lf\n", curr_ite, erreur_sur_valid, erreur_sur_app);
            pmc.reseau.save_reseau(path+"network", pmc.etha);
            min_erreur_valid = erreur_sur_valid;
        }
    
        curr_ite++;
    }
    
    fclose(f);
}
