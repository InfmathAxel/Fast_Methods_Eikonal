#include <iostream>
#include <fstream>
#include <vector>

void writeMatrixToFile(const std::vector<std::vector<std::vector<double>>>& matrix, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Unable to open file for writing." << std::endl;
        return;
    }

    for (const auto& layer : matrix) {
        for (const auto& row : layer) {
            for (const auto& elem : row) {
                outFile << elem << " ";
            }
            outFile << "\n";
        }
        outFile << "\n";
    }

    outFile.close();
}

void writeVTKFile(const std::string& tag, const std::vector<std::vector<std::vector<double>>>& T, double h) {
    std::ostringstream filename;
    filename << "/ccc/work/cont001/ocre/demutha/results/" << tag << ".vtk";

    std::ofstream file(filename.str());
    if (!file.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier " << filename.str() << std::endl;
        return;
    }

    int nx = T.size();
    int ny = T[0].size();
    int nz = T[0][0].size();

    file << "# vtk DataFile Version 3.0\n";
    file << "3D Matrix Data\n";
    file << "ASCII\n";
    file << "DATASET STRUCTURED_POINTS\n";
    file << "DIMENSIONS " << nx << " " << ny << " " << nz << "\n";
    file << "ORIGIN 0 0 0\n";
    file << "SPACING " << h << " " << h << " " << h << "\n";
    file << "POINT_DATA " << nx * ny * nz << "\n";
    file << "SCALARS T double\n";
    file << "LOOKUP_TABLE default\n";

    int compteur = 0;
    for (int k = 0; k < nz; ++k) {
        for (int j = 0; j < ny; ++j) {
            for (int i = 0; i < nx; ++i) {
                double value = T[i][j][k];
                if (std::isnan(value) || std::isinf(value)) {
                    //std::cerr << "Erreur : Valeur invalide détectée à l'indice (" << i << ", " << j << ", " << k << ")" << std::endl;
                    value = 0.0; // Remplacer par une valeur par défaut ou gérer l'erreur
                    compteur++;
                }
                file << value << " ";
            }
            file << "\n";
        }
    }

    file.close();
    std::cout << "inf : " << compteur << std::endl;
    std::cout << "Fichier VTK sauvegardé sous " << filename.str() << std::endl;
}