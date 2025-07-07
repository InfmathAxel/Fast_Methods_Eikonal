import os
import matplotlib.pyplot as plt

# Chemins des fichiers
base_path = "/results/"
tags = ["FSM", "FIM", "exact"]

# Liste des nombres de points (doit correspondre à ce qui a été utilisé dans le C++)
n_points = [1000,1500, 2000,2500, 3000,3500 ,4000,4500, 5000]

# Dictionnaire pour stocker les temps d'exécution
execution_times = {}

# Fonction pour lire les fichiers et extraire les temps d'exécution
def read_execution_times(tag):
    file_path = os.path.join(base_path, f"temps_{tag}.txt")
    if not os.path.exists(file_path):
        print(f"❌ Le fichier {file_path} n'existe pas.")
        return []

    with open(file_path, 'r') as file:
        content = file.read()
        values = content.split()
        try:
            return list(map(float, values))
        except ValueError:
            print(f"⚠️ Erreur de conversion dans {file_path}.")
            return []

# Lecture des temps pour chaque tag
for tag in tags:
    times = read_execution_times(tag)
    if len(times) != len(n_points):
        print(f"⚠️ Incohérence: {len(times)} temps pour {len(n_points)} points dans '{tag}'")
    execution_times[tag] = times

# Fonction pour tracer les courbes
def plot_execution_times():
    linestyle = ['--', ':', '-.']
    markers = ['s', 'd', '^']
    thread = [4,8,8]
    plt.figure(figsize=(10, 6))
    print(execution_times)
    print(n_points)
    for i, tag in enumerate(tags):
        if tag in execution_times:
            plt.plot(n_points, execution_times[tag],
                     label=f"{tag} avec {thread[i]} threads",
                     marker=markers[i],
                     linestyle=linestyle[i])
    
    plt.xlabel('Taille du maillage (n*n)')
    plt.ylabel('Temps d\'execution (s)')
    plt.title('Temps d\'execution parallèle pour une interface avec un seuil')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()

    # Crée le dossier 'images' s'il n'existe pas
    os.makedirs('images', exist_ok=True)
    plt.savefig('images/execution_times.png')
    plt.show()

# Tracer les courbes
plot_execution_times()
