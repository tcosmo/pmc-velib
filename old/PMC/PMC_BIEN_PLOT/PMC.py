
from Neurones import *
import numpy as np
import matplotlib.pyplot as plt
from math import *

def sigmoide(n):
    return 1/(exp(-1*n)+1)


def sigmoder(n):
    return exp(-1*n)/((exp(-1*n)+1)**2)

class PMC:
    def __init__(self, reseau, etha, base_apprentissage = ()):
        self.reseau = reseau
        self.etha = etha
        self.base_apprentissage = base_apprentissage
        self.sensibilite = []
        self.derivee = []

    def maj_der(self):
        for c in self.reseau.couches[1:]:
            self.derivee.append(c.neurones[0].transfertDer)

    def calcule_matrice_der(self,k):
        return np.diag([self.derivee[k-1](n.activation) for n in self.reseau.couches[k].neurones])

    def calcule_sensibilite(self,couple):
        self.sensibilite = []
        der = self.calcule_matrice_der(-1)
        self.sensibilite.append(-2*der*(couple[1]-self.reseau.sortie[-1]))
        k = len(self.reseau.couches)-2
        while k >= 1:
            der = self.calcule_matrice_der(k)
            poids = self.reseau.couches[k+1].matrice_poids()
            self.sensibilite.append(der*(poids)*self.sensibilite[-1])
            k -= 1
        self.sensibilite.reverse()

    def cycle_apprentissage(self, last = False):
        
        random.shuffle(self.base_apprentissage)
        bonne_base = self.base_apprentissage
        
        for couple in bonne_base:
            self.reseau.definis_entree(couple[0])
            self.reseau.calcule_sortie()
            
            if last:
                #print("Resultat : {} ; \n Attendu : {}\n".format(self.reseau.sortie[-1], couple[1]))
                continue
            #print("Resultat : {} ; \n Attendu : {}\n".format(self.reseau.sortie[-1], couple[1]))
            
            self.calcule_sensibilite(couple)
            
            for k in range(1,len(self.reseau.couches)):
                for i in range(len(self.reseau.couches[k].neurones)):
                    ajout = (-1*self.etha*(self.sensibilite[k-1]*self.reseau.sortie[k-1].T))
                    ajout = (-1*self.etha*(self.sensibilite[k-1]*self.reseau.sortie[k-1].T)[i:i+1])
                    self.reseau.couches[k].neurones[i].poids += ajout.T
                    self.reseau.couches[k].neurones[i].biais += float(self.etha*self.sensibilite[k-1][i])


#base = [(np.matrix([0.8,-0.02,-0.876]).T,np.matrix([0.8736]).T),(np.matrix([0.4,-0.823,-0.345]).T,np.matrix([0.145]).T)]
def f(x):
    return (1/2)*((exp(x)-exp(-x))/(exp(x)+exp(x)))

base = [(np.matrix([[x]]),np.matrix([[sigmoide(x)]])) for x in np.linspace(-0.95,0.95,1000)]

#print(base)

r = Reseau()
r.definis_entree(base[0][0])

r.couches.append(Couche([Neurone(sigmoide,sigmoder) for i in range(2)]))
r.couches.append(Couche([Neurone(sigmoide,sigmoder) for i in range(1)]))
r.init_reseau()

pmc = PMC(r,10,base)
pmc.maj_der()

for i in range(10):
    pmc.cycle_apprentissage()

L = np.linspace(-2.0,2.0,2000)
plt.plot(L,[sigmoide(x) for x in L],label='realite')

S = []
for x in L:
    pmc.reseau.definis_entree(np.matrix([[x]]))
    pmc.reseau.calcule_sortie()
    S.append(float(pmc.reseau.sortie[-1]))

plt.plot(L,S,label='Perceptron')
plt.legend()
plt.show()
#pmc.cycle_apprentissage(True)
