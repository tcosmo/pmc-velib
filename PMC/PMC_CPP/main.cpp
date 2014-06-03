#include <iostream>
#include <fstream>

#include "PMC.h"
#include <sqlite3.h>

using namespace std;

string PATH_DB = "../../data_mining/data_velib.sqlite";


vector<tuple<mat,mat>> base = {make_tuple(colvec({-0.4,0.6,-0.87,0.23}),colvec({0.09,0.56}))};

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
        base.push_back(make_tuple(normalize_entree(colvec({(double)ltm->tm_hour,(double)ltm->tm_min,(double)atof(argv[5]),(double)atof(argv[4])})),colvec(normalize_sortie({(double)atof(argv[1]),(double)atof(argv[2])}))));
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

    string sql = "SELECT Bike_stands, available_bikes, available_bike_stands, ID_station, Pluie, Temperature, Risque_neige, request_time FROM Station_dynamic NATURAL JOIN Station_static NATURAL JOIN Meteo WHERE ID_station = 5023";
    rc = sqlite3_exec(db,sql.c_str(),callback, (void*)data, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        fprintf(stdout,"%d %d\n", jkl, base.size());
        fprintf(stdout, "Operation done successfully\n");
    }
    
    sqlite3_close(db);

    Reseau r(get<0>(base[0]).n_rows);

    Couche C1;
    for(int i = 0 ; i < 3 ; i++)
        C1.neurones.push_back(Neurone(sigmoide,sigmoder));

    Couche C2; 
    for(int i = 0 ; i < 2 ; i++)
        C2.neurones.push_back(Neurone(sigmoide,sigmoder));
    
    Couche C3;
    for(int i = 0 ; i < 2 ; i++)
        C3.neurones.push_back(Neurone(sigmoide,sigmoder));

    r.couches.push_back(C1);
    r.couches.push_back(C2);
    r.couches.push_back(C3);
    
    PMC pmc(r,10,base); 
    
    for(int i = 0 ; i < 100 ; i++)
        pmc.cycle_apprentissage();
    
    freopen("data_pmc.data","w", stdout);
    for(int i = 0 ; i < base.size() ; i++)
    {
        mat C = get<0>(base[i]);
        pmc.reseau.calcule_sortie(C);
        cout << C << endl;
        cout << pmc.reseau.sorties.back() << endl;
        //double velo = pmc.reseau.sorties.back()[0];
        //cout << denorm(velo) << endl;
    }

    
    return 0;
}
