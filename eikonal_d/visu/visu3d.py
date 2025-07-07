import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def readMatrixFromFile(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()

    matrix = []
    layer = []
    for line in lines:
        if line.strip() == "":
            if layer:
                matrix.append(layer)
                layer = []
        else:
            row = list(map(float, line.strip().split()))
            layer.append(row)

    if layer:
        matrix.append(layer)

    return np.array(matrix)

def plotMatrices3DSubplots(matrices, titles):
    fig = plt.figure(figsize=(15, 5))

    for i, (matrix, title) in enumerate(zip(matrices, titles), 1):
        ax = fig.add_subplot(1, len(matrices), i, projection='3d')
        x, y, z = np.indices(matrix.shape)
        scatter = ax.scatter(x, y, z, c=matrix.flatten(), cmap='viridis')

        ax.set_xlabel('X')
        ax.set_ylabel('Y')
        ax.set_zlabel('Z')
        ax.set_title(title)

        # Add a color bar to each subplot
        cbar = fig.colorbar(scatter, ax=ax, shrink=0.5, aspect=5)
        cbar.set_label('Value')

    plt.tight_layout()
    plt.show()
    plot_filename = f"plot_n_400_5_{titles[0]}.png"
    plt.savefig(plot_filename, dpi=500, bbox_inches="tight")
    print("image save ")

# Lire les matrices depuis les fichiers
matrixFIM = readMatrixFromFile("results/FIM_result.txt")
matrixFSM = readMatrixFromFile("results/FSM_result.txt")
matrixex = readMatrixFromFile("results/exact_result.txt")

matrix_error_FIM = abs(matrixFIM - matrixex)
matrix_error_FSM = abs(matrixFSM - matrixex)

# Créer des sous-graphiques 3D pour chaque matrice
plotMatrices3DSubplots([matrixFIM,  matrix_error_FIM], ['FIM', 'Error FIM'])
plotMatrices3DSubplots([matrixFSM,  matrix_error_FSM], ['FSM', 'Error FSM'])
