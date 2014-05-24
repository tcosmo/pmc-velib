import sqlite3
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import datetime

conn = sqlite3.connect('data_velib.sqlite')
c = conn.cursor()

x = []
y = []
add = ""
for res in c.execute("SELECT request_time, available_bikes, Address FROM Station_dynamic NATURAL JOIN Station_static WHERE ID_station = 5021"):
    add = str(res[2])
    x.append(datetime.datetime.fromtimestamp(res[0]))
    y.append(int(res[1]))

plt.plot(x, y)
plt.title("Nombre de velos disponibles, station "+add)
plt.ylabel("Nombre de velos")
plt.grid(True)
plt.show()
