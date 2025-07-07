import os
import pandas as pd
import matplotlib.pyplot as plt

# Chemins des fichiers
base_path = "/ccc/work/cont001/ocre/demutha/results/"
tags = ["FSM", "FIM", "exact"]

# Liste des nombres de points et de sources // A MODIFIER POUR VOS DONNEES
n_points = [500,1000,1500, 2000,2500, 3000,3500,4000]
n_sources = [100, 200, 500, 1000]

# Dictionnaire pour stocker les temps d'exécution
execution_times = {tag: pd.DataFrame(index=n_points, columns=n_sources) for tag in tags}

# Fonction pour lire les fichiers et extraire les temps d'exécution
def read_execution_times(tag):
    file_path = os.path.join(base_path, f"temps_{tag}.txt")
    if not os.path.exists(file_path):
        print(f"Le fichier {file_path} n'existe pas.")
        return

    with open(file_path, 'r') as file:
        lines = file.readlines()
        for i, line in enumerate(lines):
            times = list(map(float, line.strip().split()))
            if len(times) != len(n_sources):
                print(f"Erreur: Le nombre d'éléments dans la ligne {i+1} ne correspond pas au nombre de colonnes attendu.")
                continue
            execution_times[tag].iloc[i] = times

# Lire les temps d'exécution pour chaque tag
for tag in tags:
    read_execution_times(tag)

# Fonction pour tracer les graphiques
def plot_execution_times():
    linestyle = ['dashed','dotted','dashdot']
    markers = ['s','d','^']
    for nb_source in n_sources:
    
        plt.figure(figsize=(10, 6))
        i = 0
        for tag in tags:
            plt.plot(n_points, execution_times[tag][nb_source], label=tag,marker=markers[i],linestyle = linestyle[i])
            i +=1
        plt.xlabel('Number of point n*n')
        plt.ylabel('Execution Time (s)')
        plt.title(f'Execution time for  {nb_source} sources')
        plt.legend()
        plt.grid(True)
        plt.savefig(f'images/execution_times_{nb_source}_sources.png')
        plt.close()
    for nb_point in n_points:
        plt.figure(figsize=(10, 6))
        i = 0 
        for tag in tags:
            plt.plot(n_sources, execution_times[tag].loc[nb_point], label=tag,marker=markers[i],linestyle = linestyle[i])
            i += 1 
        plt.xlabel('Number of sources')
        plt.ylabel('Execution Time (s)')
        plt.title(f'Execution Times {nb_point} points')
        plt.legend()
        plt.grid(True)
        plt.savefig(f'images/execution_times_{nb_point}_points.png')
        plt.close()

# Tracer les graphiques
plot_execution_times()
