import numpy as np
import matplotlib.pyplot as plt
import re
import os

# Liste des tags
tags = ["FIM"]

# Création de la figure pour les subplots
fig, axes = plt.subplots(len(tags), 2, figsize=(12, 4), sharex='col')

# Assurez-vous que axes est toujours traité comme un tableau 2D
if len(tags) == 1:
    axes = axes[np.newaxis, :]

# Boucle sur les tags
for i, tag in enumerate(tags):
    filename = f"results/length_10_n_1000_n_source_5_{tag}.txt"

    # Expression régulière pour extraire les nombres après "length_" et "n_"
    match = re.search(r"length_(\d+)_n_(\d+)", filename)

    if match:
        length = int(match.group(1))  # obtenir la longueur du maillage
        n = int(match.group(2))  # obtenir le nombre de nœuds dans une direction
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
    points_file = "coords_mesh.txt"  # Nom du fichier où vous avez sauvegardé vos coordonnées
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
    ax.set_title("Résultats par méthode FIM/FMM/FSM")
    ax.set_ylabel("Y")
    ax.set_xlabel("X")
    fig.colorbar(mesh, ax=ax)
    ax.legend()

    filename_exact = filename.replace(f"{tag}.txt", "exact.txt")
    if os.path.exists(filename_exact):
        print(f"✅ {tag} : Fichier trouvé")

        matrix_exact = np.loadtxt(filename_exact)
        if matrix_exact.shape != (n, n):
            raise ValueError(f"La matrice exacte pour {tag} a une forme incorrecte : {matrix_exact.shape}, attendu : ({n}, {n})")

        # Calcul et affichage de l'erreur
        error = np.abs(matrix - matrix_exact)
        ax = axes[i, 1]  # Utilisation de la deuxième colonne
        mesh_error = ax.pcolormesh(X, Y, error, shading='nearest', cmap='inferno')
        ax.set_title("Erreur absolue")
        fig.colorbar(mesh_error, ax=ax)
    else:
        print(f"⚠️ {tag} : Aucun fichier exact trouvé")

# Ajout de l'axe X uniquement pour la dernière ligne
for ax in axes[-1, :]:
    ax.set_xlabel("X")

# Sauvegarde de l'image
plot_filename = "plot_n_400_5.png"
plt.savefig(plot_filename, dpi=300, bbox_inches="tight")
print(f"📷 Graphique sauvegardé sous {plot_filename}")

plt.tight_layout()
plt.show()
