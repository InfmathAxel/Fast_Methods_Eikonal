import matplotlib.pyplot as plt
import numpy as np

n = 4000
n_source = 4000

# Fonction pour lire les temps d'exécution à partir d'un fichier
def read_times(filename):
    with open(filename, 'r') as file:
        line = file.readline()
        times = list(map(float, line.strip().split()))
    return times

# Lire les temps d'exécution pour chaque méthode
temps_FIM = np.array(read_times('FIM_threads.txt'))
temps_FSM = np.array(read_times('FSM_threads.txt'))
temps_euc = np.array(read_times('euc_threads.txt'))

# Nombre de threads
n_threads = [1, 2, 4, 8, 16, 32, 64,128]

# Calculer les speed-up
speed_up_FIM = temps_FIM[0] / temps_FIM
speed_up_FSM = temps_FSM[0] / temps_FSM
speed_up_euc = temps_euc[0] / temps_euc

# Créer le graphique pour les temps d'exécution
plt.figure(figsize=(10, 6))
plt.plot(n_threads, temps_FIM, marker='o', label='FIM')
plt.plot(n_threads, temps_FSM, marker='o', label='FSM')
plt.plot(n_threads, temps_euc, marker='o', label='Euclidien')

# Ajouter des titres et des labels
#plt.title(f'Execution time as a function of the number of threads for n_point: {n}; n_source: {n_source}')
plt.title(f'Temps d\'execution par rapport au nombres de threads pour n point : {n} et n_source: {n_source}')
plt.xlabel('Nombres de threads')
plt.ylabel('Temps d\'execution')
plt.xscale('log', base=2)  # Logarithmic scale for threads
plt.legend()
plt.grid(True)
plt.savefig("images/comparison_thread_time.png")
plt.show()

# Créer le graphique pour les speed-up
plt.figure(figsize=(10, 6))
plt.plot(n_threads, speed_up_FSM, marker='s',linestyle = 'dashed', label='FSM')
plt.plot(n_threads, speed_up_FIM, marker='d', linestyle = 'dotted', label='FIM')
plt.plot(n_threads, speed_up_euc,  marker='^', linestyle = 'dashdot', label='Euclidean')


# Ajouter des titres et des labels
plt.title(f'Speed-up par rapport au nombres de threads pour n point: {n}; n_source: {n_source}')
plt.xlabel('Nombres de threads')
plt.ylabel('Speed-up')
plt.xscale('log', base=2)  # Logarithmic scale for threads
plt.legend()
plt.grid(True)


plt.savefig("images/compar.png")
print('images save compar')
plt.show()
