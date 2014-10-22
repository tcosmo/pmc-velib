#include <vector>
#include "../../lib/tinyxml/tinyxml.h"
#include <armadillo>

#include "f_transferts.h"

using namespace arma;
using namespace std;

class Neurone
{
    public:
        Neurone(std::function<double (double)> p_transfert, function<double (double)> p_der)
        {
            transfert = p_transfert;
            transfert_der = p_der;
        }

        void calcule_activation(mat entree)
        {
            activation = poids.t()*entree - biais;
        }
        
        void calcule_sortie()
        {
            sortie = transfert(activation(0,0));
        }

        void maj_neurone(mat entree)
        {
            calcule_activation(entree);
            calcule_sortie();
        }

        mat poids, biais, activation, sortie;
        function<double (double)> transfert;
        function<double (double)> transfert_der;
};

class Couche
{
    public:
        Couche(vector<Neurone> p_neurones = vector<Neurone>())
        {
            neurones = p_neurones;
        }

        mat calcule_mat_poids()
        {
            poids = neurones[0].poids;
            for(int i = 1 ; i < neurones.size() ; i++)
            {
                poids.insert_cols(i,neurones[i].poids);
            }

            return poids;
        }


        void maj_couche(mat entree)
        {
            vector<double> p_sortie;
            for(int i=0 ; i < neurones.size() ; i++)
            {
                neurones[i].maj_neurone(entree);
                p_sortie.push_back(neurones[i].sortie(0,0));
            }
            sortie = colvec(p_sortie);
        }

        vector<Neurone> neurones;
        mat sortie;
        mat poids;
};

class Reseau
{
    public:
        Reseau(int p_format_entree=0, vector<Couche> p_couche = vector<Couche>())
        {
        	etha_read = -1;
            format_entree = p_format_entree;
            couches = p_couche;
            
            if(p_couche.size() == 0)
            {
                couches = vector<Couche> {Couche()};
                couches[0].sortie = zeros<mat>(format_entree,1);
            }
        }
		
		Reseau(string file)
		{
			etha_read = -1;
			load_reseau(file);	
		}
		
        void init_reseau()
        {
            srand(time(NULL));
            for(int i = 1 ; i < couches.size() ; i++)
            {
                couches[i].sortie = colvec(couches[i].neurones.size());
                for(Neurone &n: couches[i].neurones)
                {
                    n.poids = zeros<mat>(couches[i-1].sortie.n_rows,1);
                    n.poids.imbue([]() { 
                        float a = -0.5, b = 0.5;
                        float random = ((float) rand()) / (float) RAND_MAX;
                        float diff = b - a;
                        float r = random * diff;
                        return a + r;}); 
                    
                    n.biais = zeros<mat>(1,1);
                    n.biais.imbue([]() { 
                        float a = -0.5, b = 0.5;
                        float random = ((float) rand()) / (float) RAND_MAX;
                        float diff = b - a;
                        float r = random * diff;
                        return a + r;}); 
                    
                }
            }

        }
        
        void load_reseau(string file)
        {
			TiXmlDocument doc(file.c_str());
			if(doc.LoadFile())
			{
				auto root = doc.RootElement();
				int nbCouches = atoi(root->Attribute("couche"));
				format_entree = atoi(root->Attribute("entree"));
				etha_read = atof(root->Attribute("etha"));
				couches = vector<Couche> {Couche()};
                couches[0].sortie = zeros<mat>(format_entree,1);
                auto child = root->FirstChildElement();

                for(int iCouche = 1 ; iCouche < nbCouches ; iCouche++)
                {
                	Couche c;
                	int nb_neurones = atoi(child->Attribute("neurones"));
                	
                	c.sortie = colvec(nb_neurones);
                	auto child_child = child->FirstChildElement();
                	for(int iNeurone = 0 ; iNeurone < nb_neurones ; iNeurone++)
                	{
                		Neurone n = Neurone(sigmoide,sigmoder);
                		n.biais = zeros<mat>(1,1);
                		n.biais(0,0) = atof(child_child->Attribute("biais"));
                		
                		n.poids = zeros<mat>(couches[iCouche-1].sortie.n_rows,1);
                		auto child_child_child = child_child->FirstChildElement();
                		for(int iPoids = 0 ; iPoids < couches[iCouche-1].sortie.n_rows ; iPoids++)
                		{
                			n.poids(iPoids,0) = atof(child_child_child->Attribute("val"));
                			child_child_child = child_child_child->NextSiblingElement();
                		}
                		child_child = child_child->NextSiblingElement();
                		c.neurones.push_back(n);
                	}
                	child = child->NextSiblingElement();
                	couches.push_back(c);
                }
                
			}
        }
        
        void save_reseau(string name, double etha)
        {
            TiXmlDocument doc;  
            TiXmlElement* msg;
            TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
            doc.LinkEndChild( decl );  

            TiXmlElement * root = new TiXmlElement( "Reseau" );
            root->SetAttribute("couche",couches.size());
            root->SetAttribute("entree",format_entree);
            root->SetAttribute("sortie",couches.back().neurones.size());
            root->SetAttribute("etha",etha);
            doc.LinkEndChild( root );  
                        
            for(int i = 1 ; i < couches.size() ; i++)
            {
                TiXmlElement *couche = new TiXmlElement("Couche");
                root->LinkEndChild(couche);
                couche->SetAttribute("neurones", couches[i].neurones.size());
                for(auto n: couches[i].neurones)
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


        void calcule_sortie(mat entree)
        {
            sorties.clear();
            couches[0].sortie = entree;
            sorties.push_back(couches[0].sortie);
            for(int i = 1 ; i < couches.size() ; i++)
            {
                couches[i].maj_couche(couches[i-1].sortie);
                sorties.push_back(couches[i].sortie);
            }
        }
        
        float calcule_erreur_moyq(vector<tuple<mat,mat>> base)
        {
            float moy = 0;
            int n = 0;
            for(auto couple: base)
            {
                n++;
                calcule_sortie(get<0>(couple));
                moy += pow(get<1>(couple).at(0)-sorties.back().at(0),2);
            }
            moy /= n;
            return moy;
        }
		
    vector<Couche> couches;
    vector<mat> sorties;
    int format_entree;
    double etha_read;
};


