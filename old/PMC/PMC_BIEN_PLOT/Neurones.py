import random
import numpy as np

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
import random
def sgn():
	a = random.randrange(0,2)
	if a == 0:
		return -1
	return 1
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
            for n in self.couches[i].neurones:
            	l = [0.0]
            	for j in range(len(self.couches[i-1].sortie)):
            		l.append(sgn()*random.random()*0.6)
                n.poids = np.matrix([l])
                n.biais = sgn()*random.random()*0.6

    def calcule_sortie(self):
        self.sortie = []
        self.sortie.append(self.couches[0].sortie)
        for c in self.couches[1:]:
            c.maj_couche(self.sortie[-1])
            self.sortie.append(c.sortie)




