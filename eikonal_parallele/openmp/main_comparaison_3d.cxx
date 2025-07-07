#include "include/solveEikonal.cpp"
#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <cmath>
#include <sstream>  // Pour construire la chaîne formatée
#include <fstream>
#include <chrono>
#include <random>
#include <unordered_set>
#include "include/write_result3D.cpp"
#include "include/random_source3D.cpp"
#include "include/euclidien3D.cpp"
#include <omp.h>
#include "include/FIM3D.cpp"
#include "include/FSM3D.cpp"
#include "include/random_source3D.cpp"


/**
 * Dans ce main on execute les methodes FIM,FSM et Euclidien sur un maillage de taille n , avec n_source source et un  nombre de threads fixe (4 pour la FSM 
 * et 8 pour la FIM et euclidiens)
 * Les temps d'execution sont sauvegardé.
 */

 int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <num_n> <n1> <n2> ... <nk> <num_n_source> <n_source1> <n_source2> ... <n_sourcem>" << std::endl;
        return 1;
    }

    // Read the number of n values
    int num_n = std::stoi(argv[1]);
    if (num_n <= 0) {
        std::cerr << "The number of n values must be greater than 0." << std::endl;
        return 1;
    }

    // Read the n values
    std::vector<int> n_values(num_n);
    for (int i = 0; i < num_n; ++i) {
        n_values[i] = std::stoi(argv[2 + i]);
    }

    // Read the number of n_source values
    int num_n_source = std::stoi(argv[2 + num_n]);
    if (num_n_source <= 0) {
        std::cerr << "The number of n_source values must be greater than 0." << std::endl;
        return 1;
    }

    // Read the n_source values
    std::vector<int> n_source_values(num_n_source);
    for (int i = 0; i < num_n_source; ++i) {
        n_source_values[i] = std::stoi(argv[3 + num_n + i]);
    }

    // Print the values to verify
    std::cout << "n values: ";
    for (const auto& value : n_values) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    std::cout << "n_source values: ";
    for (const auto& value : n_source_values) {
        std::cout << value << " ";
    }
    std::cout << std::endl;


    double length = 10;

    double n_threads_fsm = 4 ;
    double n_threads_fim = 8;
    double n_threads_eucli = 8;

    std::vector<std::vector<double>> Temps_FIM(num_n, std::vector<double>(num_n_source, 0));
    std::vector<std::vector<double>> Temps_FSM(num_n, std::vector<double>(num_n_source, 0));
    std::vector<std::vector<double>> Temps_euc(num_n, std::vector<double>(num_n_source, 0));

    for (int i = 0; i < num_n; ++i) {
        int nb_point = n_values[i];
        for (int j = 0; j < num_n_source; ++j) {
            int nb_source = n_source_values[j];
            std::cout << nb_point << " " << nb_source << std::endl;
            double h = length / (nb_point - 1);
            std::vector<std::vector<std::vector<double>>> T_FIM(nb_point, std::vector<std::vector<double>>(nb_point, std::vector<double>(nb_point,std::numeric_limits<double>::infinity())));
            std::vector<std::vector<std::vector<double>>> T_FSM(nb_point, std::vector<std::vector<double>>(nb_point, std::vector<double>(nb_point,std::numeric_limits<double>::infinity())));
            std::vector<std::vector<std::vector<double>>> T_euc(nb_point, std::vector<std::vector<double>>(nb_point, std::vector<double>(nb_point,std::numeric_limits<double>::infinity())));
            std::vector<std::vector<std::vector<double>>> closest_FIM(nb_point, std::vector<std::vector<double>>(nb_point, std::vector<double>(nb_point, 0)));
            std::vector<std::vector<std::vector<double>>> closest_FSM(nb_point, std::vector<std::vector<double>>(nb_point, std::vector<double>(nb_point, 0)));
            std::vector<std::vector<std::vector<double>>> closest_euc(nb_point, std::vector<std::vector<double>>(nb_point, std::vector<double>(nb_point, 0)));
            std::vector<std::vector<std::vector<double>>> F(nb_point, std::vector<std::vector<double>>(nb_point, std::vector<double>(nb_point, 0)));
            std::vector<std::tuple<int,int,int>> Xs;
            
            generate_source3d(Xs, nb_point, nb_source, 12345, true);
            omp_set_num_threads(8);
            ;  

            double temps_fim = FIM3D(T_FIM,F, nb_point, length, Xs, n_threads_fim,closest_FIM);
            double temps_fsm = FSM3D(T_FSM, nb_point,length, Xs,closest_FSM);
            double temps_eucli = euclidien3D(T_euc, Xs, nb_point, nb_point, length,closest_euc,n_threads_eucli);
            

            Temps_FIM[i][j] = temps_fim;
            Temps_FSM[i][j] = temps_fsm;
            Temps_euc[i][j] = temps_eucli;
        }
    }

    write_files_temps(Temps_FIM, "/ccc/work/cont001/ocre/demutha/results/temps_FIM");
    write_files_temps(Temps_FSM, "/ccc/work/cont001/ocre/demutha/results/temps_FSM");
    write_files_temps(Temps_euc, "/ccc/work/cont001/ocre/demutha/results/temps_exact");

    return 0;
}
