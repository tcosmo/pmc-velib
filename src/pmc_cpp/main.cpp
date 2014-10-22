#include <iostream>
#include <fstream>

#include "PMC.h"
#include <sqlite3.h>

using namespace std;

string PATH_DB = "../../data_mining/data_velib.sqlite";


vector<tuple<mat,mat>> base = {make_tuple(colvec({-0.4,0.6,-0.87,0.23}),colvec({0.09,0.56}))};
vector<tuple<mat,mat>> base_valid = {make_tuple(colvec({-0.4,0.6,-0.87,0.23}),colvec({0.09,0.56}))};
//Heure minutes temperatures pluie

template <typename T = double>
vector<T> linspace_b(T a, T b, size_t N) {
    T h = (b - a) / static_cast<T>(N-1);
    vector<T> xs(N);
    typename vector<T>::iterator x;
    T val;
    for (x = xs.begin(), val = a; x != xs.end(); ++x, val += h)
        *x = val;
        return xs;
}


mat normalize_entree(colvec entree_non_norm)
{
    colvec N = {0.0,0.0,0.0,0.0};
    N[0] = entree_non_norm[0]*(2.0/23.0)-1;
    if(entree_non_norm[1] >= 30)
        N[1] = 1;
    else
        N[1] = 0;

    N[2] = 0.04*entree_non_norm[2]-11.52;
    if(entree_non_norm[3] > 1)
        N[3] = 1;
    else
        N[3] = entree_non_norm[3];
    return colvec(N);
}

mat normalize_sortie(colvec sortie_non_norm)
{
    colvec N = {0.0,0.0};
    N[0] = sortie_non_norm[0]*(0.90/24)+0.05;
    N[1] = sortie_non_norm[1]*(0.90/24)+0.05;
    return N;
}

//string sql = "SELECT Bike_stands, available_bikes, available_bike_stands, ID_station, Pluie, Temperature, Risque_neige, request_time FROM Station_dynamic NATURAL JOIN Station_static NATURAL JOIN Meteo LIMIT 1000";
//Heure minutes temperatures pluie

float last;
vector<int> TS;

int jkl = 0;
static int callback(void *data, int argc, char **argv, char **azColName){
    int i;
    jkl++;
    //fprintf(stderr, "%s: ", (const char*)data);
    //for(i=0; i<argc; i++){
        //printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    float f = atoi(argv[7]);
    time_t now = atoi(argv[7]);
    tm *ltm = localtime(&now);
    if(f-last >= 30*60 || last == 0)
    {
        last = f;
        if(base.size() <= 1000)
        {
            base.push_back(make_tuple(normalize_entree(colvec({(double)ltm->tm_hour,(double)ltm->tm_min,(double)atof(argv[5]),(double)atof(argv[4])})),colvec(normalize_sortie({(double)atof(argv[1]),(double)atof(argv[2])}))));
        }
    }
    //}
    //printf("\n");
    return 0;
}

int jkl2;
static int callback2(void *data, int argc, char **argv, char **azColName){
    int i;
    jkl2++;
    //fprintf(stderr, "%s: ", (const char*)data);
    //for(i=0; i<argc; i++){
        //printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    float f = atoi(argv[7]);
    time_t now = atoi(argv[7]);
    tm *ltm = localtime(&now);
    if(f-last >= 30*60 || last == 0)
    {
        last = f;
        if(base_valid.size() <= 1000)
        {
            base_valid.push_back(make_tuple(normalize_entree(colvec({(double)ltm->tm_hour,(double)ltm->tm_min,(double)atof(argv[5]),(double)atof(argv[4])})),colvec(normalize_sortie({(double)atof(argv[1]),(double)atof(argv[2])}))));
             cout << get<0>(base_valid.back())[0] << " " << get<0>(base_valid.back())[1] << " " << get<0>(base_valid.back())[2] << " " << get<0>(base_valid.back())[3] << endl;
            
            TS.push_back(atoi(argv[7]));
        }
    }
    //}
    //printf("\n");
    return 0;
}


float denorm(float v)
{
    ceil(24/0.90*v-4/3);
}


int main()
{

    sqlite3 *db;
    int rc;
    const char* data = "Callback function called";
    char *zErrMsg = 0;

    rc = sqlite3_open(PATH_DB.c_str(),&db);
    if(rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }else{
        fprintf(stderr, "Opened database successfully\n");
    }

    string sql = "SELECT Bike_stands, available_bikes, available_bike_stands, ID_station, Pluie, Temperature, Risque_neige, request_time FROM Station_dynamic NATURAL JOIN Station_static NATURAL JOIN Meteo WHERE ID_station = 5023 AND request_time BETWEEN 1401131865.282588 AND 1401287708.583986";
    base.clear();
    
    rc = sqlite3_exec(db,sql.c_str(),callback, (void*)data, &zErrMsg);



    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        fprintf(stdout,"Nombre de donnees dans la base du pmc : %d \nNombre de donnees au total : %d\n", base.size(),jkl);
        fprintf(stdout, "Operation done successfully\n");
    }
    
    sql = "SELECT Bike_stands, available_bikes, available_bike_stands, ID_station, Pluie, Temperature, Risque_neige, request_time FROM Station_dynamic NATURAL JOIN Station_static NATURAL JOIN Meteo WHERE ID_station = 5023 AND request_time BETWEEN 1401682018.0387065 AND 1401790341.953328";
    base_valid.clear();
    
    rc = sqlite3_exec(db,sql.c_str(),callback2, (void*)data, &zErrMsg);
    for(auto a: TS)
    {
        cout << a << endl;
    }
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        fprintf(stdout,"Nombre de donnees dans la base de validation : %d \nNombre de donnees au total : %d\n", base_valid.size(),jkl2);
        fprintf(stdout, "Operation done successfully\n");
    }
    sqlite3_close(db);

    Reseau r(get<0>(base[0]).n_rows);

    Couche C1;
    for(int i = 0 ; i < 5 ; i++)
        C1.neurones.push_back(Neurone(sigmoide,sigmoder));

    Couche C2; 
    for(int i = 0 ; i < 5 ; i++)
        C2.neurones.push_back(Neurone(sigmoide,sigmoder));
   
    Couche C3;
    for(int i = 0 ; i < 2 ; i++)
        C3.neurones.push_back(Neurone(sigmoide,sigmoder));

    r.couches.push_back(C1);
    //r.couches.push_back(C2);
    r.couches.push_back(C3);
   
    //base = {make_tuple(colvec({0.1,0.3,-0.4,0.5}),colvec({0.1,0.3})), make_tuple(colvec({0.3,-0.98,-0.78,0.03}),colvec({0.5,0.6}))};

    cout << "nom du fichier ? " << endl;
    string name;
    cin >> name;
    PMC pmc(r,0.01,base); 
    
    int k = 0;
    int last = 0;
    float last_err_valid  = 0.0;
    float err_valid = 0.0;
    float min_err_valid = 1000;
    int l = 0;
    while(l < 30)
    {
        if(k - last >= 1000)
        {
            l++;
            fprintf(stdout, "%d\n", k);
            last = k;
        }
        //if(pmc.erreur_moyenne.size() != 0)
          //  fprintf(stdout, "%f\n", pmc.erreur_moyenne[0]);
        pmc.cycle_apprentissage();
        last_err_valid = err_valid;
        err_valid = pmc.reseau.calcule_err(base_valid);
        if(err_valid <= min_err_valid)
        {
            l = 0;
            cout << k << " " << err_valid << " " << pmc.erreur_moyenne[0] << endl;
            min_err_valid = err_valid; 
            pmc.save_reseau(name, "[1401131865.282588, 1401287708.583986]  = apprentissage \n [1401913296.0638773, 1402081178.83343]  = validation",k, err_valid, pmc.erreur_moyenne[0]);
        }
        
        k++;
    }

    printf("Apprentissage en %d iterations\n", k);



/*
//freopen("data_pmc.data","w", stdout);
    for(int i = 0 ; i < base.size() ; i++)
    {
        mat C = get<0>(base[i]);
        pmc.reseau.calcule_sortie(C);
        cout << "Entree : " << C << endl;
        cout << "Sortie voulue : " << get<1>(base[i])[0] << endl;
        cout << "Sortie rendue : " << double(pmc.reseau.sorties.back()[0]) << endl << endl;
        //cout << pmc.reseau.sorties.back() << endl;
        //double velo = pmc.reseau.sorties.back()[0];
        //cout << denorm(velo) << endl;
    }

*/

    return 0;
}
