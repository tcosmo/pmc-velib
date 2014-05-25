import sqlite3
import urllib.request
import json

KEY_VELIB = "309f48e271264e27cf402da1f2fb380b3dd9d2d3"
URL_DECAUX = "https://api.jcdecaux.com/vls/v1/stations/"

conn = sqlite3.connect('data_velib.sqlite')
c = conn.cursor()

stationVoulue = ['05021','05023','05020','05022','05031']
for st in stationVoulue:
    url = "https://api.jcdecaux.com/vls/v1/stations/"+st+"?contract=Paris&apiKey="+KEY_VELIB
    print(url)
    req = urllib.request.urlopen("https://api.jcdecaux.com/vls/v1/stations/"+st+"?contract=Paris&apiKey="+KEY_VELIB)
    str_result = req.read().decode()
    dct_json = json.loads(str_result)
    sql_insert = 'INSERT INTO Station_static VALUES ("'+st+'","'+str(dct_json['address'])+'",'+str(dct_json['position']['lat'])+","+str(dct_json['position']['lng'])+","+str(dct_json['bike_stands'])+")"
    c.execute(sql_insert)
    print(sql_insert)
    conn.commit()
