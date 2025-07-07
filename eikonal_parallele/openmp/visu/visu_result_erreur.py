import numpy as np
import matplotlib.pyplot as plt
import re
import os

# Liste des tags
tags = ["FIM","FSM"]
n_point = 5000
# Création de la figure pour les subplots (ici, 3 lignes et 2 colonnes)
fig, axes = plt.subplots(len(tags), 2, figsize=(12, 12), sharex='col')  # Partager l'axe x entre les colonnes

# Boucle sur les tags
for i, tag in enumerate(tags):
    filename = f"/ccc/work/cont001/ocre/demutha/results/length_10_n_4000_n_source_4{tag}.txt"

    print(filename)
    # Expression régulière pour extraire les nombres après "length_" et "n_"
    match = re.search(r"length_(\d+)_n_(\d+)", filename)

    if match:
        length = int(match.group(1))  # get length of the mesh
        n = int(match.group(2))       # get number of nodes in one direction
        print(f"🔹 {tag} : length = {length}, n = {n}")
    else:
        print(f"❌ Format de fichier invalide pour {tag}")
        continue

    matrix = np.loadtxt(filename)  # Charger la matrice

    # Vérification de la taille
    n = n_point
    if matrix.shape != (n, n):
        raise ValueError(f"La matrice pour {tag} a une forme incorrecte : {matrix.shape}, attendu : ({n}, {n})")

    # Création du meshgrid
    x = np.linspace(0, length, n)
    y = np.linspace(0, length, n)
    X, Y = np.meshgrid(x, y)

    # Charger les coordonnées des points générés depuis le fichier
    points_file = "coords_mesh.txt.txt"  # Nom du fichier où tu as sauvegardé tes coordonnées
    if os.path.exists(points_file):
        coords = np.loadtxt(points_file)
        x_points, y_points = coords[:, 0], coords[:, 1]  # Séparer X et Y
    else:
        print(f"⚠️ {tag} : Fichier des coordonnées non trouvé")
        x_points, y_points = [], []

    # Affichage de la matrice avec superposition des points
    ax = axes[i, 0]
    mesh = ax.pcolormesh(X, Y, matrix, shading='nearest', cmap='viridis')
    ax.scatter(x_points, y_points, color='red', s=10, label="source")  # Ajouter les points en rouge
    ax.set_title(f"{tag} ")
    ax.set_ylabel("Y")
    fig.colorbar(mesh, ax=ax)
    ax.legend()

    filename_exact = filename.replace(f"{tag}.txt", "eucl.txt")
    if os.path.exists(filename_exact):
        print(f"✅ {tag} : Fichier trouvé")

        matrix_exact = np.loadtxt(filename_exact)
        if matrix_exact.shape != (n, n):
            raise ValueError(f"La matrice exacte pour {tag} a une forme incorrecte : {matrix_exact.shape}, attendu : ({n}, {n})")

        # Calcul et affichage de l'erreur
        error = np.abs(matrix - matrix_exact)
        ax = axes[i, 1]  # Utilisation de la deuxième colonne
        mesh_error = ax.pcolormesh(X, Y, error, shading='nearest', cmap='inferno')
        ax.set_title(f"{tag} - Error")
        fig.colorbar(mesh_error, ax=ax)
    else:
        print(f"⚠️ {tag} : Aucun fichier exact trouvé")

# Ajout de l'axe X uniquement pour la dernière ligne
for ax in axes[-1, :]:
    ax.set_xlabel("X")

# Sauvegarde de l'image
plot_filename = "plot_n_400_5_closest.png"
print(f"📷 Graphique {plot_filename}")
plt.savefig(plot_filename, dpi=300, bbox_inches="tight")
print(f"📷 Graphique sauvegardé sous {plot_filename}")

plt.tight_layout()
plt.show()
