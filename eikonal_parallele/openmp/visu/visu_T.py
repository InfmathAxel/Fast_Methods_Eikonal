import numpy as np 
import matplotlib.pyplot as plt 
import re
import os

filename = "results/length_10_n_4000_n_source_4FSM.txt"

# Expression régulière pour extraire les nombres après "length_" et "n_"
match = re.search(r"length_(\d+)_n_(\d+)", filename)

if match:
    length = int(match.group(1))  # get lenght of the mesh 
    n = int(match.group(2))       # get number of nodes in one direction 
    print(f"length = {length}, n = {n}")
else:
    print("Format de fichier invalide")

n = 2000
matrix = np.loadtxt(filename)  # load matrix from txt files 

# Check shape 
if matrix.shape != (n, n):
    raise ValueError(f"La matrice lue a une forme incorrecte : {matrix.shape}, attendu : ({n}, {n})")

# get meshgrid 
x = np.linspace(0, length, n)  
y = np.linspace(0, length, n)  
X, Y = np.meshgrid(x, y)       

# print shape 
print(f"✅ Matrice de taille : {matrix.shape}")
print(f"✅ Meshgrid X : {X.shape}, Y : {Y.shape}")

# plot the result 

plt.figure(figsize=(6, 5))
plt.pcolormesh(X, Y, matrix, shading='auto', cmap='viridis')
plt.colorbar(label="Valeurs de la matrice")
plt.title("Visualisation de la Matrice avec Meshgrid")
plt.xlabel("X")
plt.ylabel("Y")
plt.show()

filename_exact = filename.replace(".txt", "exact.txt")
if os.path.exists(filename_exact):
    print(f"🔍 Fichier exact trouvé : {filename_exact}")
    
    # Charger la matrice exacte
    matrix_exact = np.loadtxt(filename_exact)
    
    # Vérifier la taille
    if matrix_exact.shape != (n, n):
        raise ValueError(f"La matrice exacte a une forme incorrecte : {matrix_exact.shape}, attendu : ({n}, {n})")
    
    # Affichage de la matrice exacte
    plt.figure(figsize=(6, 5))
    plt.pcolormesh(X, Y, matrix_exact, shading='auto', cmap='viridis')
    plt.colorbar(label="Valeurs de la matrice exacte")
    plt.title("Visualisation de la Matrice Exacte")
    plt.xlabel("X")
    plt.ylabel("Y")
    plt.show()
    
    # Calcul et affichage de l'erreur
    error = np.abs(matrix - matrix_exact)
    plt.figure(figsize=(6, 5))
    plt.pcolormesh(X, Y, error, shading='auto', cmap='inferno')
    plt.colorbar(label="Erreur absolue")
    plt.title("Erreur entre la Matrice et la Matrice Exacte")
    plt.xlabel("X")
    plt.ylabel("Y")
    plt.show()
else:
    print("⚠️ Aucun fichier exact trouvé.")

plot_filename = "plot_FSM_5.png"
print(f"📷 Graphique {plot_filename}")
plt.savefig(plot_filename, dpi=300, bbox_inches="tight")
print(f"📷 Graphique sauvegardé sous {plot_filename}")