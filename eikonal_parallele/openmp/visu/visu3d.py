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

def plotMatrices3DSubplots(matrices, titles,colormap):
    fig = plt.figure(figsize=(15, 5))

    for i, (matrix, title,color) in enumerate(zip(matrices, titles,color), 1):
        ax = fig.add_subplot(1, len(matrices), i, projection='3d')
        x, y, z = np.indices(matrix.shape)
        scatter = ax.scatter(x, y, z, c=matrix.flatten(), cmap=color)

        ax.set_xlabel('X')
        ax.set_ylabel('Y')
        ax.set_zlabel('Z')
        ax.set_title(title)

        # Add a color bar to each subplot
        cbar = fig.colorbar(scatter, ax=ax, shrink=0.5, aspect=5)
        cbar.set_label('Value')

    plot_filename = f"images/plot_n_400_5_3d_{titles[0]}.png"
    plt.savefig(plot_filename, dpi=300, bbox_inches="tight")
    print(f"📷 Graphique sauvegardé sous {plot_filename}")

    plt.tight_layout()
    plt.show()

# Lire les matrices depuis les fichiers
matrixFIM = readMatrixFromFile("results/length_10_n_400_n_source_4FIM.txt")
matrixFSM = readMatrixFromFile("results/length_10_n_400_n_source_4FSM.txt")
matrixex = readMatrixFromFile("results/length_10_n_400_n_source_4eucl.txt")

matrix_error_FIM = abs(matrixFIM - matrixex)
matrix_error_FSM = abs(matrixFSM - matrixex)

# Créer des sous-graphiques 3D pour chaque matrice
plotMatrices3DSubplots([matrixFIM, matrix_error_FIM], ['FIM', 'Error FIM'],['viridis','inferno'])
plotMatrices3DSubplots([matrixFSM, matrix_error_FSM], ['FSM', 'Error FSM'],['viridis','inferno'])
