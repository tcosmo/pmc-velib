import random
import numpy as np
from math import *
import time
 
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
            #print("caca")
            for n in self.couches[i].neurones:
                #print("\tcaca")
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
    if x < 0 :
        x = 0
    if x-floor(x) < 0.5 :
        x = floor(x)
    else :
        x = ceil(x)
    return x
 
 
import xml.etree.ElementTree as ET
 
tree = ET.parse("network.xml")
root = tree.getroot()
n_entree = int(root.attrib["entree"])
n_couche = int(root.attrib["couche"])-1
 
 
#print(n_entree)
r = Reseau()
 
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
 
fichier = open("raw.data","r")
raw=fichier.read()
T=raw.split("\n")
c=1
l=5
abscisses_apprentissage=[]
import datetime
debapp=float(T[5].split(", ")[-1].split(")")[0])
while c==1:
    liste=T[l].split(", ")
    if len(liste)>2:
        liste2=liste[-1].split(")")
        timestamp=float(liste2[0])
        abscisses_apprentissage.append(datetime.datetime.fromtimestamp(timestamp))
        l+=1
    else :
        c=0
finapp=float(T[l-1].split(", ")[-1].split(")")[0])
abscisses_val=[]
debval=float(T[l+1].split(", ")[-1].split(")")[0])
finval=float(T[len(T)-2].split(", ")[-1].split(")")[0])
for i in range (l+1,len(T)-1):
    liste=T[i].split(", ")
    liste2=liste[-1].split(")")
    timestamp=float(liste2[0])
    abscisses_val.append(datetime.datetime.fromtimestamp(timestamp))
fichier.close()
#print((len(abscisses_apprentissage),len(abscisses_val)))
 
fichier2= open("norm.data","r")
norm=fichier2.read()
L=norm.split("\n")
#print(L)
reallibapp=[]
pmclibapp=[]
realstandapp=[]
pmcstandapp=[]
reallibval=[]
pmclibval=[]
realstandval=[]
pmcstandval=[]
for i in range(1,l-4):
    liste=L[i].split(" ")
    #print(liste)
    miam=[float(liste[j]) for j in range(n_entree)]
    realstandapp.append(denorm(float(liste[n_entree+1])))
    #print(realstandapp)
    reallibapp.append(denorm(float(liste[n_entree])))
    #print(reallibapp)
    r.definis_entree(np.matrix(miam).T)
    r.calcule_sortie()
    pmclibapp.append((denorm(float(r.sortie[-1][0]))))
    pmcstandapp.append((denorm(float(r.sortie[-1][1]))))
for i in range(l-3,len(L)):
    liste=L[i].split(" ")
    #print(liste)
    miam2=[float(liste[j]) for j in range(n_entree)]
    reallibval.append(denorm(float(liste[n_entree+1])))
    realstandval.append(denorm(float(liste[n_entree])))
    r.definis_entree(np.matrix(miam2).T)
    r.calcule_sortie()
    pmclibval.append((denorm(float(r.sortie[-1][0]))))
    pmcstandval.append((denorm(float(r.sortie[-1][1]))))
 
   
fichier2.close()
#print((len(reallibapp),len(pmclibapp),len(realstandapp),len(pmcstandapp),len(reallibval),len(pmclibval),len(realstandval),len(pmcstandval)))
       
import matplotlib.pyplot as plt
 
plt.subplot(2,1,1)
plt.plot(abscisses_apprentissage, reallibapp, label='reel')
plt.plot(abscisses_apprentissage, pmclibapp, label = 'pmc')
plt.title('Velib libres (apprentissage) du '+time.strftime("%D %H:%M", time.localtime(debapp))+" au "+time.strftime("%D %H:%M", time.localtime(finapp)))
plt.legend()
plt.subplot(2,1,2)
plt.plot(abscisses_val, reallibval, label='reel')
plt.plot(abscisses_val, pmclibval, label = 'pmc')
plt.title('Velib libres (validation) du '+time.strftime("%D %H:%M", time.localtime(debval))+" au "+time.strftime("%D %H:%M", time.localtime(finval)))
plt.legend()
 
plt.show()