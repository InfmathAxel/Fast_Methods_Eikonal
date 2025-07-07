import matplotlib.pyplot as plt
import numpy as np


def plot_results(n_points, times_fmm, times_fsm, times_fim, times_exact, n_sources):
    fig, ax1 = plt.subplots(figsize=(10, 6))

    ax1.set_xlabel("Taille du maillage (n)")
    ax1.set_ylabel("Temps (FSM, FIM, Exact) [s]", color="tab:blue")
    ax1.set_xscale("log")
    ax1.set_yscale("log")
    ax1.grid(True, which="both", linestyle="--", linewidth=0.5)

    ax2 = ax1.twinx()  # Second axe Y pour FMM
    ax2.set_ylabel("log Temps FMM [s]", color="tab:red")
    ax2.set_yscale("log")

    for i, n_source in enumerate(n_sources):
        #ax2.plot(n_points, times_fmm[i], label=f'FMM (n_source={n_source})', marker='o', color='tab:red')
        ax1.plot(n_points, times_fsm[i], label=f'FSM (n_source={n_source})', marker='s', linestyle='dashed')
        ax1.plot(n_points, times_fim[i], label=f'FIM (n_source={n_source})', marker='^', linestyle='dotted')
        ax1.plot(n_points, times_exact[i], label=f'Exact (n_source={n_source})', marker='d', linestyle='dashdot')

    fig.legend(loc="upper left", bbox_to_anchor=(0.1, 0.9))
    plt.title("Évolution du temps en fonction de la taille du maillage")
    plt.show()



# Lecture des résultats
filename = "results.txt"
nb_n_source = 8
with open(filename, 'r') as file:
        lines = file.readlines()

i = 0 
for j in range(nb_n_source):
    n_sources = []
    n_points = []
    times_fmm, times_fsm, times_fim, times_exact = [], [], [], []
    
    n_sources.append(int(lines[i].strip()))
    
    n_points = list(map(int, lines[1].strip().split()))  # Stocker comme une simple liste

    #times_fmm.append(list(map(float, lines[i+2].strip().split())))
    times_fsm.append(list(map(float, lines[i+2].strip().split())))
    times_fim.append(list(map(float, lines[i+3].strip().split())))
    times_exact.append(list(map(float, lines[i+4].strip().split())))
    

    i+=5

    # Affichage du graphique
    plot_results(n_points, times_fmm, times_fsm, times_fim, times_exact, n_sources)