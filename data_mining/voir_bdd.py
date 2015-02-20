import sqlite3
import datetime 
import time 
import matplotlib.pyplot as plt

def ok():
	return True

conn = sqlite3.connect('data_velib.sqlite')
c = conn.cursor()

def f(x):
	return x-273.15


jours = ["Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi", "Dimanche"]
station = "5022"
for jour in range(7):
	x=[]
	y=[]
	y_pluie = []
	y_temp = []
	nbJ = 0
	kk = True
	
	
	for res in c.execute("SELECT ID_station, available_bikes, Temperature, Pluie, Risque_neige, request_time FROM Station_dynamic NATURAL JOIN Meteo WHERE ID_station="+station):
		t = datetime.datetime.fromtimestamp(res[5])
		if t.weekday() == jour and kk:
			nbJ += 1
			kk = False
		if t.weekday() != jour:
			kk = True
	
	kk = False
	iPlt=1
	last_res = []
	plt.figure(figsize=(12,7))
	fig,ax1 = plt.subplots()
	for res in c.execute("SELECT ID_station, available_bikes, Temperature, Pluie, Risque_neige, request_time FROM Station_dynamic NATURAL JOIN Meteo WHERE ID_station="+station):
		t = datetime.datetime.fromtimestamp(res[5])
		if t.weekday() == jour:
			plt.title(jours[jour]+" "+time.strftime("%D", time.localtime(float(res[5]))))
			x.append(t)
			y.append(int(res[1]))
			y_pluie.append(float(res[3]))
			y_temp.append(float(res[2]))
			if kk == True and last_res != [] and res[5]-last_res[5] >= 4*60:
				print("Donnees manquantes entre : "+time.strftime("%D %H:%M:%S", time.localtime(float(last_res[5])))+" et "+time.strftime("%D %H:%M:%S", time.localtime(float(res[5]))))
			kk = True
		if t.weekday() != jour and kk:
			kk = False
			ax1.plot(x, y, 'b')
			ax1.plot(x, y_pluie, 'g')
			ax2 = ax1.twinx()
			ax2.plot(x, map(f, y_temp), 'r')
			x = []
			y = []
			y_pluie = []
			y_temp = []
			ax1.set_ylabel("Nombre de velos/Pluie")
			ax2.set_ylabel("Temperature en C")
			plt.grid(True)
			plt.legend()
			plt.savefig("fig/"+str(jour)+"-"+jours[jour]+"/"+str(iPlt))
			plt.figure(figsize=(12,7))
			fig,ax1 = plt.subplots()
			iPlt+=1
		last_res = res
	
	
