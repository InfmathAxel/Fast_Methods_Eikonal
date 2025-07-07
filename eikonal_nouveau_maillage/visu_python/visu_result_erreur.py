import numpy as np 
import matplotlib.pyplot as plt 
import re
import os

# Liste des tags
tags = ["FSM", "FIM", "FMM"]

# Création de la figure pour les subplots
fig, axes = plt.subplots(3, 4, figsize=(16, 12))  # 4 colonnes pour ajouter le graphe "closest"

# Boucle sur les tags
for i, tag in enumerate(tags):
    filename = f"results/length_10_n_500_n_source_4_{tag}.txt"

    # Expression régulière pour extraire les nombres après "length_" et "n_"
    match = re.search(r"length_(\d+)_n_(\d+)", filename)

    if match:
        length = int(match.group(1))  # Longueur du maillage
        n = int(match.group(2))       # Nombre de nœuds
        print(f"🔹 {tag} : length = {length}, n = {n}")
    else:
        print(f"❌ Format de fichier invalide pour {tag}")
        continue

    matrix = np.loadtxt(filename)  # Charger la matrice

    # Vérification de la taille
    if matrix.shape != (n, n):
        raise ValueError(f"La matrice pour {tag} a une forme incorrecte : {matrix.shape}, attendu : ({n}, {n})")

    # Création du meshgrid
    x = np.linspace(0, length, n)  
    y = np.linspace(0, length, n)  
    X, Y = np.meshgrid(x, y)

    # Charger les coordonnées des points générés depuis le fichier
    points_file = "coords_mesh.txt"
    if os.path.exists(points_file):
        coords = np.loadtxt(points_file)
        x_points, y_points = coords[:, 0], coords[:, 1]  # Séparer X et Y
    else:
        print(f"⚠️ {tag} : Fichier des coordonnées non trouvé")
        x_points, y_points = [], []

    # Affichage de la matrice avec superposition des points
    ax = axes[i, 0]
    mesh = ax.pcolormesh(X, Y, matrix, shading='nearest', cmap='viridis')
    ax.scatter(x_points, y_points, color='red', s=10, label="Points")
    ax.set_title(f"{tag} - Matrice avec points")
    ax.set_xlabel("X")
    ax.set_ylabel("Y")
    fig.colorbar(mesh, ax=ax)
    ax.legend()

    # Affichage de la matrice exacte
    filename_exact = filename.replace(f"{tag}.txt", "exact.txt")
    if os.path.exists(filename_exact):
        print(f"✅ {tag} : Fichier exact trouvé")

        matrix_exact = np.loadtxt(filename_exact)
        if matrix_exact.shape != (n, n):
            raise ValueError(f"La matrice exacte pour {tag} a une forme incorrecte : {matrix_exact.shape}, attendu : ({n}, {n})")

        ax = axes[i, 1]
        mesh_exact = ax.pcolormesh(X, Y, matrix_exact, shading='nearest', cmap='viridis')
        ax.set_title(f"{tag} - Matrice Exacte")
        ax.set_xlabel("X")
        ax.set_ylabel("Y")
        fig.colorbar(mesh_exact, ax=ax)

        # Affichage de l'erreur
        error = np.abs(matrix - matrix_exact)
        ax = axes[i, 2]
        mesh_error = ax.pcolormesh(X, Y, error, shading='nearest', cmap='inferno')
        ax.set_title(f"{tag} - Erreur")
        ax.set_xlabel("X")
        ax.set_ylabel("Y")
        fig.colorbar(mesh_error, ax=ax)
    else:
        print(f"⚠️ {tag} : Aucun fichier exact trouvé")

    # 🔹 Ajout du graphe "closest_{tag}.txt"
    filename_closest = filename.replace(f"{tag}.txt", f"closest_{tag}.txt")
    if os.path.exists(filename_closest):
        print(f"🔍 Fichier closest trouvé : {filename_closest}")

        # Charger les données
        with open(filename_closest, 'r') as f:
            lines = f.readlines()
        
        # Lire la première ligne (indices sous forme de tuples)
        index_tuples = [eval(t) for t in re.findall(r"\(\d+,\d+\)", lines[0])]
        
        # Lire la matrice restante
        matrix_closest = np.loadtxt(lines[1:])

        x_points = [x[j] for i, j in index_tuples]  # Colonnes (j)
        y_points = [y[i] for i, j in index_tuples]  # Lignes (i)
        # Vérifier la taille de la matrice
        if matrix_closest.shape != (n, n):
            raise ValueError(f"❌ La matrice closest a une forme incorrecte : {matrix_closest.shape}, attendu : ({n}, {n})")

        # Affichage de la matrice closest
        ax = axes[i, 3]
        mesh_closest = ax.pcolormesh(X, Y, matrix_closest, shading='nearest', cmap='plasma')
        ax.set_title(f"{tag} - Matrice Closest")
        ax.set_xlabel("X")
        ax.set_ylabel("Y")
        fig.colorbar(mesh_closest, ax=ax)
        # Affichage des indices
        print(f"📌 Indices lus depuis closest_FMM.txt : {index_tuples}")

    else:
        print("⚠️ Aucun fichier closest trouvé.")

plot_filename = "images/plot_results_lenght_10_n_500_n_source_4.png"
plt.savefig(plot_filename, dpi=300, bbox_inches="tight")
print(f"📷 Graphique sauvegardé sous {plot_filename}")

plt.tight_layout()
plt.show()
