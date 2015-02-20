import random
import numpy as np
from math import *

def sigmoide(n):
    return 1/(exp(-1*n)+1)


def sigmoder(n):
    return exp(-1*n)/((exp(-1*n)+1)**2)

class Neurone:
    def __init__(self, transfert, transfertDer):
        self.transfert = transfert
        self.transfertDer = transfertDer
        self.poids = np.matrix([])
        self.biais = 0
        self.activation = 0
        self.sortie = 0

    def calcule_activation(self,entree):
        self.activation = float(self.poids.transpose()*entree)-self.biais

    def calcule_sortie(self):
        self.sortie = self.transfert(self.activation)

    def maj_neurone(self,entree):
        self.calcule_activation(entree)
        self.calcule_sortie()

class Couche:
    def __init__(self,neurones=[]):
        self.neurones = neurones
        self.sortie = np.matrix([[0] for i in range(len(self.neurones))])
    
    def sortie_couche0(self,entree):
        self.sortie = entree

    def matrice_poids(self):
        if len(self.neurones) == 0:
            return 0

        matrice = self.neurones[0].poids
        for n in self.neurones[1:]:
            matrice = np.hstack((matrice,n.poids))
        
        return matrice

    def calcule_sortie(self):
        for n in self.neurones:
            self.sortie = np.matrix([[n.sortie] for n in self.neurones])
    
    def maj_couche(self,entree):
        for n in self.neurones:
            n.maj_neurone(entree)
            self.calcule_sortie()

class Reseau:
    def __init__(self,couches=[],init_poids=(lambda x: 2)):
        self.couches = couches
        self.init_poids = init_poids
        self.sortie = []

        if self.couches == []:
            self.couches = [Couche()]
   
    def definis_entree(self, entree):
        self.couches[0].sortie_couche0(entree)

    def init_reseau(self):
        for i in range(1,len(self.couches)):
            print("caca")
            for n in self.couches[i].neurones:
                print("\tcaca")
                n.poids = np.matrix([[0.1] for j in range(len(self.couches[i-1].sortie))])
                n.biais = -0.1

    def calcule_sortie(self):
        self.sortie = []
        self.sortie.append(self.couches[0].sortie)
        for c in self.couches[1:]:
            c.maj_couche(self.sortie[-1])
            self.sortie.append(c.sortie)


from math import ceil,floor
def denorm(y) :
    x = 24*(y-0.05)/0.90
    if x-floor(x) < 0.5 :
        x = floor(x)
    else :
        x = ceil(x)
    return x


from math import ceil,floor
def denorm(y) :
    x = 24*(y-0.05)/0.90
    if x < 0 :
        x = 0
    if x-floor(x) < 0.5 :
        x = floor(x)
    else :
        x = ceil(x)
    return x

#print("Nom fichier ?")
#fic = input()

import datetime


import matplotlib.pyplot as plt

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

#deb = 1401859869.920636
#fin = 1401978929.201392
#deb = 1401661738.0096178 
#fin = 1401757168.442515

deb = 1416837628.66707-86400*7*2
fin = 1416924028.797585-86400*7*2

station = [["5020","5021","5022"],["5023","5031",""]]
k = 1
m = 0
import time
for i in range(2):
    for j in range(3):
        if station[i][j] == "5022":
            x = []
            y = []
            add = ""
            cap = 0
            k += 1
            
            last = 0
            for res in c.execute("SELECT request_time, available_bikes, Address, Bike_stands FROM Station_dynamic NATURAL JOIN Station_static WHERE ID_station = "+station[i][j]+" AND request_time BETWEEN "+str(deb)+" AND "+str(fin)):
            
            #AND request_time BETWEEN "+str(deb)+ " AND "+str(fin)):
                add = str(res[2])
                cap = int(res[3])
                if res[0] - last > 60*30 or last == 0:
                    m +=1
                    last = res[0]
                    x.append(datetime.datetime.fromtimestamp(res[0]))
                    y.append(int(res[1]))
            plt.plot(x, y, label="realite")
            plt.title("From "+time.strftime("%D %H:%M", time.localtime(deb))+" To "+time.strftime("%D %H:%M", time.localtime(fin)))
            plt.ylabel("Nombre de velos")
            plt.grid(True)
            #plt.savefig(SCREEN_PATH+station[i][j]+"-27-05-2014.png")
plt.legend()
plt.show()



