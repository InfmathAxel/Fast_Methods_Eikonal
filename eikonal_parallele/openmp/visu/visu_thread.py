import matplotlib.pyplot as plt
import numpy as np

n = 3000
n_source = 1000

# Function to read execution times from a file
def read_times(filename):
    with open(filename, 'r') as file:
        line = file.readline()
        times = list(map(float, line.strip().split()))
    return times

# Read execution times for each method
temps_FIM = read_times('results/temps_FIM_threads.txt')
temps_FSM = read_times('results/temps_FSM_threads.txt')
temps_euc = read_times('results/temps_euc_threads.txt')

# Number of threads
n_threads = [1, 2, 4, 8, 16, 32, 64, 128]
linestyle = ['dashed','dotted','dashdot']
markers = ['s','d','^']
# Create the plot
plt.figure(figsize=(10, 6))
plt.plot(n_threads, temps_FSM, marker='s',linestyle = 'dashed', label='FSM')
plt.plot(n_threads, temps_FIM, marker='d', linestyle = 'dotted', label='FIM')
plt.plot(n_threads, temps_euc, marker='^', linestyle = 'dashdot', label='Euclidean')

# Add titles and labels
plt.title(f'Execution time as a function of the number of threads for n_point: {n}; n_source: {n_source}')
plt.xlabel('Number of threads')
plt.ylabel('Execution time (s)')
plt.xscale('log', base=2)  # Logarithmic scale for threads
plt.legend()
plt.grid(True)
plt.savefig("images/comparison_thread")

# Show the plot
plt.show()
