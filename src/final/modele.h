#ifndef DEF_MODELE_H
#define DEF_MODELE_H

#include <iostream>//voir où se trouve function
#include <vector>
#include "f_transferts.h"

#include <armadillo>

using namespace std;
using namespace arma;

/*
   Un neurone est représenté par sa fonction de transfert et la dérivée de celle-ci.
   On créer un alias de type par soucis de concision.
 */

typedef pair<function<double (double)>,function<double (double)>> t_neurone;

typedef pair<vector<double>,double> t_exemple; //Represente un exemple, association d'une entrée et d'une sortie

double carre(double x)
{
    return x*x;
}   


/* Renvoie le vecteur des premieres composantes d'un vecteur de couple*/
vector<double> vec_first(vector<pair<double,double>> a)
{
    vector<double> toReturn;
    for(auto b: a)
        toReturn.push_back(b.first);
    return toReturn;
}

/* Surcharge de l'opérateur + pour implémenter l'addition vectorielle */
vector<double> operator + (const vector<double> &a, const vector<double> &b)
{
    vector<double> sum;
    for(int i = 0 ; i < a.size() ; i++)
        sum.push_back(a[i]+b[i]);
    return sum;
}

vector<vector<double>> operator - (const vector<vector<double>> &a, mat b)
{
    vector<vector<double>> diff;
    int k = 0;
    for(int i = 0 ; i < a.size() ; i++)
    {
        diff.push_back(vector<double>());
        for(int j = 0 ; j < a[i].size() ; j++)
        {
            diff[i].push_back(a[i][j]-b(k,0));
            k++;
        }
    }
    return diff;
}

mat inverse(mat m)
{
    return inv_sympd(m);//via Cholesky spécialement conçu pour sym def pos
}

/*
   Classe plutôt générique de modèle neuronal.
   Plutôt car représente uniquement la structure de réseau de neurones à 1 couche cachée
   à 1 neurone de sortie, utilisée dans ce TIPE.

 */

class modele
{

    public:

        modele(int nb_entrees, int complexite) : nb_entrees(nb_entrees), complexite(complexite)
        {
            /* Remplissage de la couche cachée */
            for(int i_cache = 0 ; i_cache < complexite ; i_cache++)
            {
                neurones.push_back(make_pair(sigmoide,sigmoder));
                poids.push_back(vector<double>());
                for(int j = 0 ; j < nb_entrees ; j++)
                    poids[i_cache].push_back(1.6);
            }

            /*Neurone de sortie*/
            neurones.push_back(make_pair(lin,linder));
            poids.push_back(vector<double>());
            for(int j = 0 ; j < complexite ; j++)
                poids[complexite].push_back(-0.32);
        }


        /* Calcul le potentiel d'activation du neurone i selon une entree */
        double activation(int i, vector<double> entree)
        {   
            double seuil = 0;
            for(int j = 0 ; j < poids[i].size() ; j++)
                seuil += poids[i][j]*entree[j];
            return seuil;
        }   

        /*
           Renvoit le vecteur contenant pour chaque neurone (f(s),s)
           où s est le seuil associé à une entrée donnée et f la fonction de transfert du neurone.
         */
        vector<pair<double,double>> sortie_par_neurone(vector<double> entree)
        {
            vector<pair<double,double>> sortie_neurones;
            for(int i = 0 ; i < neurones.size() ; i++)
            {
                auto f = neurones[i].first;
                double seuil = (i != complexite) ? activation(i,entree) : activation(i,vec_first(sortie_neurones));
                sortie_neurones.push_back(make_pair(f(seuil),seuil));
            }
            return sortie_neurones;
        } 

        double calcule_sortie(vector<double> entree)
        {
            auto sortie_neurones = sortie_par_neurone(entree);
            return sortie_neurones[complexite].first;    
        }


        /* Fonction perte : l'erreur quad commise sur un exemple i.e la donnée d'une entrée
           et de la sortie que l'on veut y associer.
         */
        double pi(t_exemple exemple)
        {
            return carre(exemple.second-calcule_sortie(exemple.first));
        }

        int nb_poids()
        {
            return (nb_entrees+1)*complexite;
        }

        /* Implémentation de la rétropropagation 
           Calcul le gradient de la fonction de perte associée à un exemple.
           Calcul un vecteur d'intérêt dans le calcul de l'approximation de la matrice Hessienne. 
        */
        pair<mat,mat> grad_hess_pi(t_exemple exemple)
        {   
            mat grad = zeros<colvec>(nb_poids());
            mat vec_hess = zeros<colvec>(nb_poids());
            vector<pair<double,double>> sortie_neurones = sortie_par_neurone(exemple.first);

            /*Utile pour tous les neurones : dpi/dvs
              Dérivée de la fonction de perte associée à un exemple donné selon
              le potentiel du neurone de sortie évaluée en w.
             */
            double e = exemple.second-sortie_neurones[complexite].first;//erreur de prédiction 
            double dpi_dvs = -2*(e)*neurones[complexite].second(sortie_neurones[complexite].second);

            int k = 0;//compteur total sur les poids
            /*Pour les neurones cachés*/
            for(int i = 0 ; i < neurones.size()-1 ; i++)
                for(int j = 0 ; j < poids[i].size() ; j++)
                {
                    /*Valeur de dpi/dwij évalulée en w
                      On utilise : dpi/dwij = dpi/dvi * dvi/dwij
                      dvi/dwij = exemple.first[j]
                      Et : dpi/dvi = dpi/dvs * dvs/dvi
                      dpi/dvs déja calculé
                      Et : dvs/dvi = poids[complexite][i]*f'(vi)
                     */
                    double dvs_dvi = poids[complexite][i]*neurones[i].second(sortie_neurones[i].second);
                    double dpi_dwij = exemple.first[j]*dpi_dvs*dvs_dvi;

                    grad(k,0) = dpi_dwij;
                    vec_hess(k,0) = grad(k,0)/(-2*e);
                    k++;
                } 

            /*Pour le neurone de sortie*/
            for(int i = 0 ; i < neurones.size()-1 ; i++)
            {
                double dvs_dwsi = neurones[i].first(sortie_neurones[i].second);
                double dpi_dwsi = dpi_dvs*dvs_dwsi;
                grad(k,0) = dpi_dwsi;
                vec_hess(k,0) = grad(k,0)/(-2*e);
                k++;
            }
            return make_pair(grad,vec_hess);
        }

        /* Fonction à minimiser : somme des pi des exemples */
        double cout(vector<t_exemple> ensemble_ex)
        {
            double s = 0;
            for(auto ex: ensemble_ex)
                s += pi(ex);
            return s;
        }


        /* Par linéarité, le calcul de grad_cout est une addition vectorielle.
           Avec l'approximation faite sur la matrice Hessienne au voisinage d'un minimum
           on peut faire d'une pierre deux coups et calculer la hessienne en même temps que le
           gradient.
        */
        pair<mat,mat> grad_hess_cout(vector<t_exemple> ensemble_ex)
        {
            mat grad = zeros<colvec>(nb_poids());
            mat approx_hess = zeros<mat>(nb_poids(),nb_poids());
            for(int i = 0 ; i < ensemble_ex.size()  ; i++)
            {
                pair<mat,mat> grad_hess = grad_hess_pi(ensemble_ex[i]);
                grad += grad_hess.first;
                approx_hess += grad_hess.second*trans(grad_hess.second);
            }
            return make_pair(grad,approx_hess);
        }


        /* implémentation de l'algorithme de Levenberg-Marquardt */

        void optimise(int n_max, vector<t_exemple> ensemble_ex)
        {
            double mu = 3.14; // C'est pour rigoler
            double r = 10; // facteur d'échelle
            double last_cout = cout(ensemble_ex);
            bool doit_mieux = false;
            for(int iter = 0 ; iter < n_max ; iter++)
            {
                pair<mat,mat> grad_hess = grad_hess_cout(ensemble_ex);
                printf("Position avant : %lf %lf\n", poids[0][0], poids[1][0]); 
                std::cout << grad_hess.first << grad_hess.second;
                vector<vector<double>> ancien_poids = poids;

                poids = poids - inverse(grad_hess.second+mu*eye<mat>(nb_poids(),nb_poids()))*grad_hess.first;
                double cur_cout = cout(ensemble_ex);
                printf("Position apres : %lf %lf %lf\n", poids[0][0], poids[1][0], cur_cout); 
                printf("Couts : %lf %lf\n",  cur_cout, last_cout);
                if(cur_cout < last_cout)
                {
                    if(!doit_mieux && iter != n_max-1)
                        poids = ancien_poids;
                    else
                    {
                        printf("ADJUGE\n");
                        doit_mieux = false;
                        continue;
                    }
                    mu /= 10;
                    last_cout = cur_cout;
                    continue;
                } 

                if(cur_cout > last_cout)
                {
                    poids = ancien_poids;
                    last_cout = cur_cout;
                    mu *= 10;
                    doit_mieux = true;
                    continue;
                }
            }

        }

        int nb_entrees; //Nombre de variables d'entrée du modèle
        int complexite; //Nombre de neurones cachés

        vector<vector<double> > poids;    
        vector<t_neurone> neurones;


};


#endif
