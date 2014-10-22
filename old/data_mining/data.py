import sqlite3
import json
import time
import datetime
import urllib.request
import xmltodict
import xml.etree.ElementTree as ET

FREQ_VELIB = 2
KEY_VELIB = "309f48e271264e27cf402da1f2fb380b3dd9d2d3"
URL_DECAUX = "https://api.jcdecaux.com/vls/v1/stations/"

FREQ_METEO = 24
URL_METEO = "http://www.infoclimat.fr/public-api/gfs/xml?_ll=48.85341,2.3488&_auth=AxlXQFUrBCYALVdgUyVXfgJqAzYPeVVyBXkFZl86An8DaFY3B2dQNlQ6UC0EKwA2WXQPbFtgUGAGbQpyAXMAYQNpVztVPgRjAG9XMlN8V3wCLANiDy9VcgVuBWNfLAJkA2dWLAdnUDZUJVAzBDUAMll1D3BbZVBsBmUKbQFrAGsDYlczVTMEZQBwVypTZldgAjYDNg8zVT4FMwVnXzoCaAM0VmcHYlAwVCVQOwQ8ADdZYg9oW2RQbQZnCnIBcwAaAxNXLlV2BCQAOldzU35XNgJvAzc%3D&_c=1ca27ca86a88862fa7b9f7fad69a495d"

conn = sqlite3.connect('data_velib.sqlite')
c = conn.cursor()

dct_static = {}
stationVoulue = []


for row in c.execute("SELECT * FROM Station_static"):
    stationVoulue.append(row[0])
print("Frequence de requete, en minute ? (min 1)")
FREQ_VELIB = int(input())

print("Temps de mining ? (en min, pas plus d'un jour)")
MINING_TIME = int(input())

NB_REQ = MINING_TIME//FREQ_VELIB
print("Nombre de requetes : {}".format(NB_REQ))

print("\nDEBUT REQUETE METEO")
req_meteo = urllib.request.urlopen(URL_METEO)
str_meteo = req_meteo.read().decode()
fic_meteo = open("curr_meteo.xml", "w")
fic_meteo.write(str_meteo)
fic_meteo.close()
print("FIN REQUETE METEO")

dct_json_meteo = {}

tree = ET.parse("curr_meteo.xml")
root = tree.getroot()

liste_key = []

for child in root:
    if child.tag == "echeance":
        key = child.attrib['timestamp']
        key = key[:len(key)-4]
        dct_json_meteo[key] = {}
        liste_key.append(time.mktime(datetime.datetime.strptime(key, "%Y-%m-%d %H:%M:%S").timetuple()))
        dct_json_meteo[key]['Date'] = key
        for sous_child in child:
            if sous_child.tag == "temperature":
                for ss_child in sous_child:
                    if ss_child.attrib['val']=='sol':
                        dct_json_meteo[key]['Temperature'] = ss_child.text
            if sous_child.tag == "pluie":
                dct_json_meteo[key]['Pluie'] = sous_child.text
            if sous_child.tag == "risque_neige":
                dct_json_meteo[key]['Neige'] = "TRUE" if sous_child.text == 'oui' else "FALSE"

def bon_meteo():
    curr_time = time.time()
    min_diff = abs(liste_key[0]-curr_time)
    key_diff = liste_key[0]

    for k in liste_key:
        if abs(k-curr_time) < min_diff:
            key_diff = k
            min_diff = abs(k-curr_time)
    meteo_intere = datetime.datetime.fromtimestamp(int(key_diff)).strftime("%Y-%m-%d %H:%M:%S")
    return meteo_intere

curr_meteo = ""

curr_time = time.time()
last_time = 0


print("\nDEBUT DU MINING\n")

iReq = 1
while iReq <= NB_REQ:
    if curr_meteo != bon_meteo():
        print("DEBUT CHANGEMENT METEO REF")
        curr_meteo = bon_meteo()
        
        sql_meteo = 'INSERT INTO Meteo ("Temperature","Pluie","Date","Risque_neige") VALUES ('+dct_json_meteo[curr_meteo]['Temperature']+","+dct_json_meteo[curr_meteo]['Pluie']+","+str(time.mktime(datetime.datetime.strptime(dct_json_meteo[curr_meteo]['Date'], "%Y-%m-%d %H:%M:%S").timetuple()))+',"'+dct_json_meteo[curr_meteo]['Neige']+'")'
        print(sql_meteo)
        print("FIN CHANGEMENT METEO REF")
        c.execute(sql_meteo)
        conn.commit()
    
    curr_time = time.time()
    if curr_time-last_time > FREQ_VELIB*60:
        print("{} REQUETE {}/{} DEBUT".format(datetime.datetime.fromtimestamp(curr_time).strftime("%Y-%m-%d %H:%M:%S"), iReq, NB_REQ))
        last_time = time.time()
        id_meteo = 0
        for t in c.execute("SELECT * FROM Meteo"):
            id_meteo += 1
        for st in stationVoulue:
            req = urllib.request.urlopen("https://api.jcdecaux.com/vls/v1/stations/"+str(st)+"?contract=Paris&apiKey="+KEY_VELIB)
            str_result = req.read().decode()
            dct_velib_json = json.loads(str_result)
            sql_velib = 'INSERT INTO Station_dynamic ("ID_station","ID_meteo","available_bike_stands","available_bikes","last_update", "request_time") VALUES ('+str(st)+','+str(id_meteo)+","+str(dct_velib_json['available_bike_stands'])+","+str(dct_velib_json['available_bikes'])+","+str(dct_velib_json['last_update']/1e3)+","+str(curr_time)+")"
            print(sql_velib)
            c.execute(sql_velib)
            conn.commit()
        curr_time = time.time()
        print("{} REQUETE {}/{} FIN \n".format(datetime.datetime.fromtimestamp(curr_time).strftime("%Y-%m-%d %H:%M:%S"),
        iReq, NB_REQ)) 
        iReq += 1

print("FIN DU MINING SUCCES")


