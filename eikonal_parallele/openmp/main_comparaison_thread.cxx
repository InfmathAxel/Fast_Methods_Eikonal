#include "include/solveEikonal.cpp"
#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <fstream>
#include <chrono>
#include <random>
#include <unordered_set>
#include "include/write_result.cpp"
#include "include/random_source.cpp"
#include "include/euclidien.cpp"
#include <omp.h>
#include "include/FIM.cpp"
#include "include/FIM_new.cpp"
#include "include/FSM.cpp"
#include "include/random_source3D.cpp"



/**
 * Dans ce main on execute les methodes FIM,FSM et Euclidien sur un maillage de taille n , avec n_source source et une liste de nombre de threads n_threads
 * Les temps d'execution sont sauvegardé. 
 */
int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <n_source> <n> <n_threads1> [<n_threads2> ... <n_threadsk>]" << std::endl;
        return 1;
    }

    int n_source = std::stoi(argv[1]);
    int n = std::stoi(argv[2]);
    int num_threads_count = argc - 3;
    std::vector<int> n_threads(num_threads_count);

    for (int i = 0; i < num_threads_count; ++i) {
        n_threads[i] = std::stoi(argv[3 + i]);
    }

    double length = 10;
    double seuil = 2; 
    std::vector<std::vector<double>> Temps_FIM(1, std::vector<double>(num_threads_count, 0));
    std::vector<std::vector<double>> Temps_FSM(1, std::vector<double>(num_threads_count, 0));
    std::vector<std::vector<double>> Temps_euc(1, std::vector<double>(num_threads_count, 0));

    for (int i = 0; i < num_threads_count; ++i) {
        int num_threads = n_threads[i];
        omp_set_num_threads(num_threads);

        double h = length / (n - 1);
        std::vector<std::vector<double>> T_FIM(n, std::vector<double>(n, std::numeric_limits<double>::infinity()));
        std::vector<std::vector<double>> T_FSM(n, std::vector<double>(n, std::numeric_limits<double>::infinity()));
        std::vector<std::vector<double>> T_euc(n, std::vector<double>(n, std::numeric_limits<double>::infinity()));
        std::vector<std::vector<double>> closest_FIM(n, std::vector<double>(n, 0));
        std::vector<std::vector<double>> closest_FSM(n, std::vector<double>(n, 0));
        std::vector<std::vector<double>> closest_euc(n, std::vector<double>(n, 0));
        std::vector<std::vector<double>> F(n, std::vector<double>(n, 1.0));
        std::vector<std::pair<int, int>> Xs;

        generate_source_sinus2d(Xs, n,12345, true);

        double temps_fim = FIM2D_new(T_FIM, F, n, length,seuil, Xs, num_threads,closest_FIM);
        double temps_fsm = FSM2D_seuil(T_FSM, F, n, length,seuil, Xs, num_threads);
        double temps_eucli = euclidien2d_seuil(T_euc, Xs, n, n, length,seuil, num_threads,closest_euc);

        Temps_FIM[0][i] = temps_fim;
        Temps_FSM[0][i] = temps_fsm;
        Temps_euc[0][i] = temps_eucli;
    }

    write_files_temps(Temps_FIM, "FIM_threads");
    write_files_temps(Temps_FSM, "FSM_threads");
    write_files_temps(Temps_euc, "euc_threads");

    return 0;
}