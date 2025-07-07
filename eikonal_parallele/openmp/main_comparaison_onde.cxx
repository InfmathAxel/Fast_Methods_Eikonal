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
#include "include/write_result.cpp"
#include "include/random_source.cpp"
#include "include/euclidien.cpp"
#include <omp.h>
#include "include/FIM.cpp"
#include "include/FSM.cpp"
#include "include/random_source3D.cpp"
#include "include/FIM_new.cpp"

/**
 * Dans ce main on execute les methodes FIM,FSM et Euclidien sur un maillage de taille n , avec n_source source et un  nombre de threads fixe (4 pour la FSM 
 * et 8 pour la FIM et euclidiens)
 * Les temps d'execution sont sauvegardé.
 */

 int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <num_n> <n1> <n2> ... <nk>" << std::endl;
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

    

    // Print the values to verify
    std::cout << "n values: ";
    for (const auto& value : n_values) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    


    double length = 10;

    double n_threads_fsm = 4 ;
    double n_threads_fim = 8;
    double n_threads_eucli = 8;

    std::vector<double> Temps_FIM;
    std::vector<double> Temps_FSM;
    std::vector<double> Temps_euc;

    double seuil = 3;
    for (int i = 0; i < num_n; ++i) {
        int nb_point = n_values[i];
            double h = length / (nb_point - 1);
            std::vector<std::vector<double>> T_FIM(nb_point, std::vector<double>(nb_point, std::numeric_limits<double>::infinity()));
            std::vector<std::vector<double>> T_FSM(nb_point, std::vector<double>(nb_point, std::numeric_limits<double>::infinity()));
            std::vector<std::vector<double>> T_euc(nb_point, std::vector<double>(nb_point, std::numeric_limits<double>::infinity()));
            std::vector<std::vector<double>> closest_FIM(nb_point, std::vector<double>(nb_point, 0));
            std::vector<std::vector<double>> closest_FSM(nb_point, std::vector<double>(nb_point, 0));
            std::vector<std::vector<double>> closest_euc(nb_point, std::vector<double>(nb_point, 0));
            std::vector<std::vector<double>> F(nb_point, std::vector<double>(nb_point, 1.0));
            std::vector<std::pair<int, int>> Xs;
            
            generate_source_sinus2d(Xs, nb_point, 12345, true);
            omp_set_num_threads(8);
            double temps_fsm = FSM2D_seuil(T_FSM,F,nb_point,length,seuil,Xs,4);
            double temps_fim = FIM2D_new(T_FIM,F,nb_point,length,seuil,Xs,n_threads_fim,closest_FIM,false);  
            double temps_eucli = euclidien2d_seuil(T_euc,Xs,nb_point,nb_point,length,seuil,n_threads_eucli,closest_euc);

            Temps_FIM.push_back(temps_fim);
            Temps_FSM.push_back(temps_fsm);
            Temps_euc.push_back(temps_eucli);
        
    }

    write_files_temps2(Temps_FIM, "/ccc/work/cont001/ocre/demutha/results/temps_FIM");
    write_files_temps2(Temps_FSM, "/ccc/work/cont001/ocre/demutha/results/temps_FSM");
    write_files_temps2(Temps_euc, "/ccc/work/cont001/ocre/demutha/results/temps_exact");

    return 0;
}
