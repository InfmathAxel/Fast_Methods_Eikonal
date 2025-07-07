import numpy as np 
import matplotlib.pyplot as plt 
import re
import os
#Initialisation des valeurs 
length = 10
n = 400

# Lire les points à partir du fichier Xs
filename_xs = "resultat_exemple2/Xs.txt"
points = np.loadtxt(filename_xs)



#on lit le fichier txt
filename = "resultat_exemple2/FIM_exemple2.txt"
matrix = np.loadtxt(filename)  

#on vérifie si on a bien pu lire le fichier 
if not os.path.isfile(filename):
    raise FileNotFoundError(f"Le fichier {filename} n'existe pas.Veuillez utiliser exemple2.cxx d'abord")

if matrix.shape != (n, n):
    raise ValueError(f"La matrice lue a une forme incorrecte : {matrix.shape}, attendu : ({n}, {n})")

#initialisation du meshgrid pour la visu
x = np.linspace(0, length, n)  
y = np.linspace(0, length, n)  
X, Y = np.meshgrid(x, y)       

#visualisation avec sauvegarde de l'image 
plt.figure(figsize=(6, 5))
plt.pcolormesh(X, Y, matrix, shading='auto', cmap='viridis')
plt.scatter(points[:, 0], points[:, 1], color='red', marker='o', label='Points')
plt.colorbar(label="Valeurs de la matrice")
plt.title("Visualisation de la Matrice FIM ")
plt.xlabel("X")
plt.ylabel("Y")
plt.savefig("resultat_exemple2/FIM.png")
plt.show()

print("Plot sauvegardé dans résultat_exemple2/FIM.png")





#on lit le fichier txt
filename = "resultat_exemple2/FSM_exemple2.txt"
matrix = np.loadtxt(filename)  

#on vérifie si on a bien pu lire le fichier 
if not os.path.isfile(filename):
    raise FileNotFoundError(f"Le fichier {filename} n'existe pas.Veuillez utiliser exemple2.cxx d'abord")

if matrix.shape != (n, n):
    raise ValueError(f"La matrice lue a une forme incorrecte : {matrix.shape}, attendu : ({n}, {n})")

#initialisation du meshgrid pour la visu
x = np.linspace(0, length, n)  
y = np.linspace(0, length, n)  
X, Y = np.meshgrid(x, y)       

#visualisation avec sauvegarde de l'image 
plt.figure(figsize=(6, 5))
plt.pcolormesh(X, Y, matrix, shading='auto', cmap='viridis')
plt.scatter(points[:, 0], points[:, 1], color='red', marker='o', label='Points')
plt.colorbar(label="Valeurs de la matrice")
plt.title("Visualisation de la Matrice FSM ")
plt.xlabel("X")
plt.ylabel("Y")
plt.savefig("resultat_exemple2/FSM.png")
plt.show()

print("Plot sauvegardé dans résultat_exemple2/FSM.png")

print("Le résultat attendu est un gradient de couleur en largeur,avec une onde sur la droite si la seed n'a pas été changé.")