import numpy as np 
import matplotlib.pyplot as plt 
import re
import os
tags =  [ "FIM"]
for tag in tags:
    filename = f"results/length_10_n_1000_n_source_5_{tag}.txt"
    # Expression régulière pour extraire les nombres après "length_" et "n_"
    match = re.search(r"length_(\d+)_n_(\d+)", filename)

    if match:
        length = int(match.group(1))  # get lenght of the mesh 
        n = int(match.group(2))       # get number of nodes in one direction 
        print(f"length = {length}, n = {n}")
    else:
        print("Format de fichier invalide")


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
    points_file = "coords_mesh.txt"  # Nom du fichier où tu as sauvegardé tes coordonnées
    if os.path.exists(points_file):
        coords = np.loadtxt(points_file)
        x_points, y_points = coords[:, 0], coords[:, 1]  # Séparer X et Y
    else:
        print(f"⚠️ {tag} : Fichier des coordonnées non trouvé")
        x_points, y_points = [], []

    
    plt.figure(figsize=(6, 5))
    plt.pcolormesh(X, Y, matrix, shading='nearest', cmap='viridis')
    plt.colorbar(label="Valeurs de la matrice")
    plt.scatter(x_points, y_points, color='red', s=10, label="source")
    plt.title(f"Résultats avec FIM/FMM/FSM")
    plt.xlabel("X")
    plt.ylabel("Y")
    plt.show()
    plot_filename = "plot_n_1000_5.png"
    plt.savefig(plot_filename, dpi=400, bbox_inches="tight")

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
        plt.pcolormesh(X, Y, matrix_exact, shading='nearest', cmap='viridis')
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
        plot_filename = "plot_n_1000_5.png"
        plt.savefig(plot_filename, dpi=400, bbox_inches="tight")
    else:
        print("⚠️ Aucun fichier exact trouvé.")



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
        print(matrix_closest)
        # Vérifier la taille de la matrice
        if matrix_closest.shape != (n, n):
            raise ValueError(f"❌ La matrice closest a une forme incorrecte : {matrix_closest.shape}, attendu : ({n}, {n})")

        # Affichage de la matrice closest
        plt.figure(figsize=(6, 5))
        plt.pcolormesh(X, Y, matrix_closest, shading='nearest', cmap='plasma')
        plt.scatter(x_points, y_points, color='red', marker='o', s=15, label="Indices Closest")  # Ajout des points
    
        plt.title(f"Visualisation de la Matrice avec {tag} Closest")
        plt.xlabel("X")
        plt.ylabel("Y")
        plt.show()

        # Affichage des indices
        print(f"📌 Indices lus depuis closest_FMM.txt : {index_tuples}")

    else:
        print("⚠️ Aucun fichier closest trouvé.")