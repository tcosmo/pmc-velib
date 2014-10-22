#include <cmath>
#include "../../lib/tinyxml/tinyxml.h"

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

            erreur_moyenne.clear();

            for(auto a: get<1>(base_appretissage[0])) 
                erreur_moyenne.push_back(0.0);

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
            for(auto couple: base_appretissage)
            {
                int i = 0;
                reseau.calcule_sortie(get<0>(couple));
                for(auto a: get<1>(couple))
                {
                    erreur_moyenne[i] += abs(reseau.sorties.back()[i]-a);
                    i++;
                }
            }
            int i = 0;
            for(auto a: get<1>(base_appretissage[0]))
            {
                erreur_moyenne[i] /= base_appretissage.size();
                i++;
            }

            //cout << "e " << erreur_moyenne[0] << endl;
        }

        void save_reseau(string name, string s_comment, int k, float quad, float valabs)
        {
            TiXmlDocument doc;  
            TiXmlElement* msg;
            TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
            doc.LinkEndChild( decl );  

            TiXmlElement * root = new TiXmlElement( "Reseau" );
            root->SetAttribute("couche",reseau.couches.size());
            root->SetAttribute("entree",get<0>(base_appretissage[0]).n_rows);
            root->SetAttribute("sortie",reseau.couches.back().neurones.size());
            doc.LinkEndChild( root );  
            
            std::ostringstream q,z,maman;
            q << k;
            z << quad;
            maman << valabs;

            TiXmlComment * comment = new TiXmlComment();
            comment->SetValue((s_comment+"\n"+q.str()+" "+z.str()+" "+maman.str()).c_str());  
            root->LinkEndChild( comment );  
            
            for(int i = 1 ; i < reseau.couches.size() ; i++)
            {
                TiXmlElement *couche = new TiXmlElement("Couche");
                root->LinkEndChild(couche);
                couche->SetAttribute("neurones", reseau.couches[i].neurones.size());
                for(auto n: reseau.couches[i].neurones)
                {
                    TiXmlElement *neurone = new TiXmlElement("Neurone");
                    couche->LinkEndChild(neurone);
                    std::ostringstream s;
                    s << n.biais.at(0);
                    neurone->SetAttribute("biais",s.str().c_str());
                    
                    
                    for(int i_poids = 0 ; i_poids < n.poids.n_rows ; i_poids++)
                    {
                        TiXmlElement *poids = new TiXmlElement("Poids");
                        neurone->LinkEndChild(poids);
                        std::ostringstream ss;
                        ss << n.poids.at(i_poids);
                        poids->SetAttribute("val", ss.str().c_str());
                    }
                }
            }
            
            doc.SaveFile( (name+".xml").c_str() );  

        }

        int compte_poids()
        {
            int k = 0;
            for(int i = 1 ; i < reseau.couches.size()  ; i++)
            {
                for(auto n : reseau.couches[i].neurones)
                    k += n.poids.n_rows;
            }
            return k;
        }

        Reseau reseau;
        double etha;
        vector<tuple<mat,mat>> base_appretissage;
        vector<mat> sensibilite;
        vector<double> erreur_moyenne;
};
