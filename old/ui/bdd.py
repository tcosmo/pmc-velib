import sqlite3

def genere_donnees(dico):
    conn = sqlite3.connect('../data_mining/data_velib.sqlite')
    c = conn.cursor()
    
    req_app = "SELECT Bike_stands, available_bikes, available_bike_stands, ID_station, Pluie, Temperature, Risque_neige, request_time FROM Station_dynamic NATURAL JOIN Station_static NATURAL JOIN Meteo WHERE ID_station = 50"+dico["station"]+" AND request_time BETWEEN "+dico["app_deb"]+" AND "+dico["app_fin"]
 
    req_val = "SELECT Bike_stands, available_bikes, available_bike_stands, ID_station, Pluie, Temperature, Risque_neige, request_time FROM Station_dynamic NATURAL JOIN Station_static NATURAL JOIN Meteo WHERE ID_station = 50"+dico["station"]+" AND request_time BETWEEN "+dico["val_deb"]+" AND "+dico["val_fin"]
    
    horodotage = float(dico["horodotage"])
    curr_temps = 0.0
    res_app_horodotage = []
    for res in c.execute(req_app):
        if res[7]-curr_temps >= horodotage*60 or curr_temps == 0.0:
            curr_temps = res[7]
            res_app_horodotage.append(res)

    print(res_app_horodotage)
