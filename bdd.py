import sqlite3
import time

def norm_heure(ts):
    h = int(time.strftime('%H',time.localtime(ts)))
    return h*(2.0/23.0)-1

def norm_minute(ts):
    m = int(time.strftime('%M',time.localtime(ts)))
    return m*(2.0/59.0)-1

def norm_temp(temp):
    return 0.04*temp-11.52

def norm_pluie(pl,M):
    return pl*(2.0/M)-1

def genere_donnees(dico):
    conn = sqlite3.connect('data_mining/data_velib.sqlite')
    c = conn.cursor()
    M=0
    req_max_pluie = "SELECT Max(Pluie) FROM Meteo"
    for a in c.execute(req_max_pluie):
        M = a[0]
    req_app = "SELECT Bike_stands, available_bikes, available_bike_stands, ID_station, Pluie, Temperature, Risque_neige, request_time FROM Station_dynamic NATURAL JOIN Station_static NATURAL JOIN Meteo WHERE ID_station = 50"+dico["station"]+" AND request_time BETWEEN "+dico["app_deb"]+" AND "+dico["app_fin"]
 
    req_val = "SELECT Bike_stands, available_bikes, available_bike_stands, ID_station, Pluie, Temperature, Risque_neige, request_time FROM Station_dynamic NATURAL JOIN Station_static NATURAL JOIN Meteo WHERE ID_station = 50"+dico["station"]+" AND request_time BETWEEN "+dico["val_deb"]+" AND "+dico["val_fin"]
    horodotage = 0
    if dico["horodotage"] != "autre":
        horodotage = float(dico["horodotage"])
    else:
        horodotage = float(dico["autre"])
    curr_temps = 0.0
    res_app_horodotage = []
    for res in c.execute(req_app):
        if res[7]-curr_temps >= horodotage*60 or curr_temps == 0.0:
            curr_temps = res[7]
            res_app_horodotage.append(res)
    
    curr_temps = 0.0
    res_val_horodotage = []
    for res in c.execute(req_val):
        if res[7]-curr_temps >= horodotage*60 or curr_temps == 0.0:
            curr_temps = res[7]
            res_val_horodotage.append(res)

    import os
    curr_file = 1
    for a in os.listdir('results'):
        if a != '.DS_Store':
            curr_file += 1
    path = 'results/'+str(curr_file)+'/'
    os.mkdir('results/'+str(curr_file))

    fic = open(path+'raw.data','w')
    fic.write(req_app+"\n"+req_val+"\n")
    fic.write("Criteres : Heure Minute")
    if dico["temp"]:
        fic.write(" Temperature")
    if dico["pluie"]:
        fic.write(" Pluie")
    if dico["neige"]:
        fic.write(" Neige")

    fic.write("\n")

    fic.write("horodotage : "+str(horodotage)+"\n")
    
    fic.write(str(len(res_app_horodotage))+"\n")
    for a in res_app_horodotage:
        fic.write(str(a)+"\n")


    fic.write(str(len(res_val_horodotage))+"\n")
    for a in res_val_horodotage:
        fic.write(str(a)+"\n")


    fic.close()

    fic = open(path+'norm.data','w')
    first = True
    n_entree = 2    
    fic.write(str(len(res_app_horodotage))+"\n")
    for a in res_app_horodotage:
        ts = a[7]
        h = norm_heure(ts)
        fic.write(str(h)+" ")
        m = norm_minute(ts)
        fic.write(str(m)+" ")
        if dico["temp"]:
            if first:
                n_entree += 1
            temp = norm_temp(a[5])
            fic.write(str(temp)+" ")
        if dico["pluie"]:
            if first:
                n_entree += 1
            pluie = norm_pluie(a[4],M)
            fic.write(str(pluie)+" ")
        
        norm_velo_libre = a[1]*(0.90/a[0])+0.05
        fic.write(str(norm_velo_libre)+" ")
        norm_stands_libre = a[2]*(0.90/a[0])+0.05
        fic.write(str(norm_stands_libre))
        fic.write("\n")    
        first = False

    fic.write(str(len(res_val_horodotage)))
    for a in res_val_horodotage:
        fic.write("\n")    
        ts = a[7]
        h = norm_heure(ts)
        fic.write(str(h)+" ")
        m = norm_minute(ts)
        fic.write(str(m)+" ")
        if dico["temp"]:
            temp = norm_temp(a[5])
            fic.write(str(temp)+" ")
        if dico["pluie"]:
            pluie = norm_pluie(a[4],M)
            fic.write(str(pluie)+" ")
        
        norm_velo_libre = a[1]*(0.90/a[0])+0.05
        fic.write(str(norm_velo_libre)+" ")
        norm_stands_libre = a[2]*(0.90/a[0])+0.05
        fic.write(str(norm_stands_libre))
    fic.close()

    from subprocess import call
    args = ["./init_reseau", path, str(n_entree), dico["fac_app"], str(2+dico["couche_cachee"])]+dico["neurone_couche"]
    call(args)
    call(["cp","src/init_reseau/learn.sh",path])
