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
    
    
import xml.etree.ElementTree as ET

tree = ET.parse("test3-base-0.5.xml")
root = tree.getroot()
n_entree = int(root.attrib["entree"])
n_couche = int(root.attrib["couche"])-1

r = Reseau()

r.definis_entree(np.matrix([0.0 for j in range(n_entree)]).T)


for child in root :
    c = Couche([])
    for gchild in child :
        n = Neurone(sigmoide,sigmoder)
        L = []
        #print(gchild.attrib)
        for poids in gchild:
            #print(poids.attrib)
            L.append(float(poids.attrib['val']))
        n.poids = np.matrix(L).T
        n.biais = float(gchild.attrib['biais'])
        c.neurones.append(n)
    r.couches.append(c)
#r.init_reseau()
r.definis_entree(np.matrix([0.0,0.9,0.8,0.3]).T)
r.calcule_sortie()
print(denorm(float(r.sortie[-1][0])))

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

f = open("y_data.data","r")
s = f.read()
s.split("\n")
liste = []
for i in range (len(s.split("\n"))) :
    if s.split("\n")[i].split(" ") != [''] :
        liste.append(list(map(float,s.split("\n")[i].split(" "))))


Tab = [0]*(len(liste))
for k in range (len(liste)) :
    r.definis_entree(np.matrix(liste[k]).T)
    r.calcule_sortie()
    Tab[k] = denorm(float(r.sortie[-1][0]))
   

# 1401682018.0387065
# 1401790341.953328

f = open("x_data.data","r")
s = f.read()
X = []
for i in range(len(s.split("\n"))):
    if s.split("\n")[i] != '':
        X.append(datetime.datetime.fromtimestamp(float(s.split("\n")[i])))

import matplotlib.pyplot as plt

plt.plot(X,Tab,label="Percetron")
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

deb = 1401682018.0387065
fin = 1401790341.953328

station = [["5020","5021","5022"],["5023","5031",""]]
k = 1
m = 0
import time
for i in range(2):
    for j in range(3):
        if station[i][j] == "5023":
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



