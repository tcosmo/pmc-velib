import sqlite3
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import datetime

conn = sqlite3.connect('data_velib.sqlite')
c = conn.cursor()


station = [["5020","5021","5022"],["5023","5031",""]]
k = 1
for i in range(2):
    for j in range(3):
        if station[i][j] != "":
            x = []
            y = []
            add = ""
            cap = 0
            plt.subplot(2,3,k)
            k += 1
            for res in c.execute("SELECT request_time, available_bikes, Address, Bike_stands FROM Station_dynamic NATURAL JOIN Station_static WHERE ID_station = "+station[i][j]):
                add = str(res[2])
                cap = int(res[3])
                x.append(datetime.datetime.fromtimestamp(res[0]))
                y.append(int(res[1]))
                plt.plot(x, y)
                plt.title(add+", "+str(cap))
                plt.ylabel("Nombre de velos")
                plt.grid(True)

plt.show()
