import sqlite3
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import datetime

conn = sqlite3.connect('data_velib.sqlite')
c = conn.cursor()

x = []
y = []

for res in c.execute("SELECT last_update, available_bikes FROM Station_dynamic WHERE ID_station = 5022"):
    print(res[0])
    x.append(datetime.datetime.fromtimestamp(res[0]))
    y.append(int(res[1]))

plt.plot(x, y)
plt.title("Page impressions on example.com")
plt.ylabel("Page impressions")
plt.grid(True)
plt.show()
