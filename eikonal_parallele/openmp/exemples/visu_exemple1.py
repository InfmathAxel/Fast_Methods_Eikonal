import numpy as np 
import matplotlib.pyplot as plt 
import re
import os
#Initialisation des valeurs 
length = 10
n = 400

#on lit le fichier txt
filename = "resultat_exemple1/FIM_exemple1.txt"
matrix = np.loadtxt(filename)  

#on vérifie si on a bien pu lire le fichier 
if not os.path.isfile(filename):
    raise FileNotFoundError(f"Le fichier {filename} n'existe pas.Veuillez utiliser exemple1.cxx d'abord")

if matrix.shape != (n, n):
    raise ValueError(f"La matrice lue a une forme incorrecte : {matrix.shape}, attendu : ({n}, {n})")

#initialisation du meshgrid pour la visu
x = np.linspace(0, length, n)  
y = np.linspace(0, length, n)  
X, Y = np.meshgrid(x, y)       

#visualisation avec sauvegarde de l'image 
plt.figure(figsize=(6, 5))
plt.pcolormesh(X, Y, matrix, shading='auto', cmap='viridis')
plt.colorbar(label="Valeurs de la matrice")
plt.title("Visualisation de la Matrice FIM ")
plt.xlabel("X")
plt.ylabel("Y")
plt.savefig("resultat_exemple1/FIM.png")
plt.show()

print("Plot sauvegardé dans résultat_exemple1/FIM.png")





#on lit le fichier txt
filename = "resultat_exemple1/FSM_exemple1.txt"
matrix = np.loadtxt(filename)  

#on vérifie si on a bien pu lire le fichier 
if not os.path.isfile(filename):
    raise FileNotFoundError(f"Le fichier {filename} n'existe pas.Veuillez utiliser exemple1.cxx d'abord")

if matrix.shape != (n, n):
    raise ValueError(f"La matrice lue a une forme incorrecte : {matrix.shape}, attendu : ({n}, {n})")

#initialisation du meshgrid pour la visu
x = np.linspace(0, length, n)  
y = np.linspace(0, length, n)  
X, Y = np.meshgrid(x, y)       

#visualisation avec sauvegarde de l'image 
plt.figure(figsize=(6, 5))
plt.pcolormesh(X, Y, matrix, shading='auto', cmap='viridis')
plt.colorbar(label="Valeurs de la matrice")
plt.title("Visualisation de la Matrice FSM ")
plt.xlabel("X")
plt.ylabel("Y")
plt.savefig("resultat_exemple1/FSM.png")
plt.show()

print("Plot sauvegardé dans résultat_exemple1/FSM.png")

print("Le résultat attendu est une découpe d'un carré en 4 carrés.")