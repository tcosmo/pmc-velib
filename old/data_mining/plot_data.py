import sqlite3
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import datetime

conn = sqlite3.connect('data_velib.sqlite')
c = conn.cursor()

SCREEN_PATH = "../screens/"

#deb = 1401163234.4032855
#fin = 1401249661.726192

#deb = 1401131865.282588
#fin = 1401769903.750158

#deb = 1401661738.0096178 
#fin = 1401757168.442515

deb=1401131865.282588
fin=1401287708.583986

station = [["5020","5021","5022"],["5023","5031",""]]
k = 1
m = 0
for i in range(2):
    for j in range(3):
        if station[i][j] == "5023":
            x = []
            y = []
            add = ""
            cap = 0
            plt.figure()
            k += 1
            
            last = 0
            for res in c.execute("SELECT request_time, available_bikes, Address, Bike_stands FROM Station_dynamic NATURAL JOIN Station_static WHERE ID_station = "+station[i][j]+" AND request_time BETWEEN "+str(deb)+" AND "+str(fin)):
            
            #AND request_time BETWEEN "+str(deb)+ " AND "+str(fin)):
                add = str(res[2])
                cap = int(res[3])
                if res[0] - last > 30*60 or last == 0:
                    m +=1
                    last = res[0]
                    x.append(datetime.datetime.fromtimestamp(res[0]))
                    y.append(int(res[1]))
                plt.plot(x, y)
                plt.title(add+", "+str(cap))
                plt.ylabel("Nombre de velos")
                plt.grid(True)
            #plt.savefig(SCREEN_PATH+station[i][j]+"-27-05-2014.png")
print(m)
plt.show()
