#include <sstream>  // Pour construire la chaîne formatée
#include <fstream>
#include <random>
#include "include/FSM2D.hpp"
#include "include/FIM2D.hpp"
#include "include/random_source2D.hpp"
#include "include/write_result2D.hpp"
#include "include/euclidien2D.hpp"

void printMatrix3D(const std::vector<std::vector<std::vector<double>>>& matrix) {
    for (size_t layer = 0; layer < matrix.size(); ++layer) {
        std::cout << "Layer " << layer + 1 << ":\n";
        for (const auto& row : matrix[layer]) {
            for (const auto& elem : row) {
                if (elem == std::numeric_limits<double>::infinity()) {
                    std::cout << "inf ";
                } else {
                    std::cout << elem << " ";
                }
            }
            std::cout << std::endl;
        }
        std::cout << std::endl; // Séparation entre les couches
    }
}


/**
 * Dans ce main on compare les temps d'execution
 */
int main(int argc, char* argv[]) {
    int n_source = 0;
    int nb_n = 0;
    int* n_tab = nullptr;

    if (argc == 3) {
        n_source = std::stoi(argv[1]);
        nb_n = 1;
        n_tab = new int[nb_n];
        n_tab[0] = std::stoi(argv[2]);
        std::cout << "Valeur de n_source et n: " << n_source << " " << n_tab[0] << std::endl;
        if (n_source > n_tab[0]) {
            std::cout << "usage: n_source < n" << std::endl;
            return 1;
        }
    } else if (argc > 3) {
        n_source = std::stoi(argv[1]);
        nb_n = argc - 2;
        n_tab = new int[nb_n];
        for (int i = 0; i < nb_n; i++) {
            n_tab[i] = std::stoi(argv[2 + i]);
        }
    } else {
        std::cout << "Usage: " << argv[0] << " <n_source> <n1> [<n2> ... <nk>]" << std::endl;
        return 1;
    }

   

    std::ostringstream filename;
    filename << "result" <<  ".txt";
    std::ofstream file(filename.str(), std::ios::out);
    if (!file) {
        std::cerr << "Erreur lors de l'ouverture du fichier!" << std::endl;
        return 1;
    }

    double length = 10;
    double t = 0;

    for (int i = 0; i < nb_n; i++) {
        int n = n_tab[i];
        std::vector<double> time_fmm;
        std::vector<double> time_fim;
        std::vector<double> time_fsm;
        std::vector<double> time_exact;

        std::vector<std::vector<double>> T_FMM(n, std::vector<double>(n, std::numeric_limits<double>::infinity()));
        std::vector<std::vector<double>> closest_FMM(n, std::vector<double>(n, 0));
        std::vector<std::vector<double>> T_FIM(n, std::vector<double>(n, std::numeric_limits<double>::infinity()));
        std::vector<std::vector<double>> closest_FIM(n, std::vector<double>(n, 0));
        std::vector<std::vector<double>> T_exact(n, std::vector<double>(n, std::numeric_limits<double>::infinity()));
        std::vector<std::vector<double>> closest_exact(n, std::vector<double>(n, 0));
        std::vector<std::vector<double>> closest_FSM(n, std::vector<double>(n, 0));
        std::vector<std::vector<double>> T_FSM(n, std::vector<double>(n, std::numeric_limits<double>::infinity()));
        std::vector<std::pair<int, int>> Xs;
        generate_source_sinus(Xs, n);

        t = FMM(T_FMM, n, length, Xs, closest_FMM);
        time_fmm.push_back(t);
        t = FSM2D(T_FSM, n, length, Xs, closest_FSM);
        time_fsm.push_back(t);
        t = FIM2D(T_FIM, n, length, Xs, closest_FIM);
        time_fim.push_back(t);
        t = euclidien(T_exact, Xs, n, n, length, closest_exact);
        time_exact.push_back(t);
        std::cout << n << " " << n << std::endl;

        saveResults(file, n_source, nb_n, n_tab, time_fmm, time_fsm, time_fim, time_exact);
    }
    file.close();

    delete[] n_tab;
    return 0;
}