#include <cstdio>
#include <cstdlib>

#include "../pmc_cpp/PMC.h"

using namespace std;

int main()
{
    Reseau r("hihi.xml");

    vector<tuple<mat,mat>> base_app;
    freopen("app", "r", stdin);
    vector<double> e;
    for(int i = 0 ; i < 1500*8 ; i++)
    {
    	double a;
        scanf("%lf", &a);
        e.push_back(a);
        
        if(i%8 == 7)
        {
            r.calcule_sortie(colvec(e));
            base_app.push_back(make_tuple(colvec(e),r.couches[2].sortie));
            for(int j = 0 ; j < 8 ; j++)
            {
            	printf("\t%lf\n", e[j]);	
            }
            
            
            printf("%lf\n",r.couches[2].sortie(0,0));
            e.clear();
        }
    }

    for(auto c: r.couches) 
    {
    	for(auto n: c.neurones)
    	{
    		cout << n.poids << endl;	
    	}	
    	
    } 
    
    Reseau r_eleve("hihi.xml");  
    r_eleve.init_reseau();
    
    for(auto c: r_eleve.couches) 
    {
    	for(auto n: c.neurones)
    	{
    		cout << n.poids << endl;	
    	}	
    	
    } 
    
    PMC pmc(r_eleve, 0.1, base_app);
	for(int i = 0 ; i < 1000 ; i++)
	{
		printf("%d\n", i);
		pmc.cycle_apprentissage();
		
	}
	
	printf("%lf\n", r_eleve.calcule_erreur_moyq(base_app));
	
	for(auto c: r_eleve.couches) 
    {
    	for(auto n: c.neurones)
    	{
    		cout << n.poids << endl;	
    	}	
    	
    } 

}
