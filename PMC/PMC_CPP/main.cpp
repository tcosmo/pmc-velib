#include <iostream>
#include <fstream>

#include "PMC.h"
#include <sqlite3.h>

using namespace std;

string PATH_DB = "../../data_mining/data_velib.sqlite";

//ID_station, Date, Bike_stands, Pluie, Temperature, neige


vector<tuple<mat,mat>> base = {make_tuple(colvec({-0.4,0.6,-0.87,0.23,0.34,-0.12,-0.83}),colvec({0.09,0.56}))};

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


//string sql = "SELECT Bike_stands, available_bikes, available_bike_stands, ID_station, Pluie, Temperature, Risque_neige, request_time FROM Station_dynamic NATURAL JOIN Station_static NATURAL JOIN Meteo LIMIT 1000";
static int callback(void *data, int argc, char **argv, char **azColName){
    int i;
    fprintf(stderr, "%s: ", (const char*)data);
    for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    
        //base.push_back(make_tuple(colvec({normal_id_station[argv[3]], normal_date}),colvec({})));
    }
    printf("\n");
    return 0;
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

    string sql = "SELECT Bike_stands, available_bikes, available_bike_stands, ID_station, Pluie, Temperature, Risque_neige, request_time FROM Station_dynamic NATURAL JOIN Station_static NATURAL JOIN Meteo LIMIT 1000";
    rc = sqlite3_exec(db,sql.c_str(),callback, (void*)data, &zErrMsg);
    
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
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
    
    for(int i = 0 ; i < 10000 ; i++)
        pmc.cycle_apprentissage();
    
    
    mat C = get<0>(base[0]);
    cout << "Entree " << C << endl;
    pmc.reseau.calcule_sortie(C);
    cout << "sortie" << endl;
    cout << pmc.reseau.sorties.back();
    cout << "nb poids " << pmc.compte_poids(); 



    

    return 0;
}
