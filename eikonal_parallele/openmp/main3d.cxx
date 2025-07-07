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
#include "include/FIM3D.cpp"
#include "include/FSM3D.cpp"
#include "include/write_result3D.cpp"
#include "include/euclidien3D.cpp"
#include "include/random_source3D.cpp"


/**
 * Main pour tester les méthodes FIM,FSM et eucli en parallèle ET 3D.
 * Sur des maillages de tailles N : n*n*n ,de longueur lenght sur tout les axes avec n_source source.
 * Avec un calcul d'erreur et une écritue de fichier vtk pour la visualisation  
 */

 int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <n_source> <n> <n_threads> " << std::endl;
        return 1;
    }
    int n_thread = std::stoi(argv[3]);
    int n_source = std::stoi(argv[1]);
    int n = std::stoi(argv[2]);
    
    double length = 10;
    double h = length/(n-1);
    
    
    std::vector<std::vector<std::vector<double>>> F(n, std::vector<std::vector<double>>(n, std::vector<double>(n, 1.0)));
    std::vector<std::vector<std::vector<double>>> T_fsm(n, std::vector<std::vector<double>>(n, std::vector<double>(n, std::numeric_limits<double>::infinity())));
    std::vector<std::vector<std::vector<double>>> T_fim(n, std::vector<std::vector<double>>(n, std::vector<double>(n, std::numeric_limits<double>::infinity())));
    std::vector<std::vector<std::vector<double>>> T_exact(n, std::vector<std::vector<double>>(n, std::vector<double>(n, std::numeric_limits<double>::infinity())));
    std::vector<std::vector<std::vector<double>>> T_erreur(n, std::vector<std::vector<double>>(n, std::vector<double>(n, std::numeric_limits<double>::infinity())));
    std::vector<std::vector<std::vector<double>>> closest_fsm(n, std::vector<std::vector<double>>(n, std::vector<double>(n, 0)));
    std::vector<std::vector<std::vector<double>>> closest_fim(n, std::vector<std::vector<double>>(n, std::vector<double>(n, 0)));
    std::vector<std::vector<std::vector<double>>> closest_exacte(n, std::vector<std::vector<double>>(n, std::vector<double>(n, 0)));
    std::vector<std::vector<std::vector<double>>> closest_erreur(n, std::vector<std::vector<double>>(n, std::vector<double>(n, 0)));

    std::vector<std::tuple<int,int,int>> Xs;
    
    
    int seed = 12345;
    

    bool give_seed = true;
    generate_source3d(Xs,n,n_source,seed,give_seed);
    std::cout << "Xs generer " << std::endl;
    std::cout << "Pour N points : " << n*n*n << " Et n_source :" << n_source << std::endl;
    
    
    FSM3D(T_fsm,n,length,Xs,closest_fsm);
    FIM3D(T_fim,F,n,length,Xs,n_thread,closest_fim);  
    euclidien3D(T_exact,Xs,n,n,length,closest_exacte,n_thread);
    
    erreur(T_erreur,T_exact,T_fim);
    erreur_closest(closest_erreur,closest_exacte,closest_fim);
    
    
    
    writeVTKFile("FIM_200_n" ,T_fim,h);
    writeVTKFile("FSM_200_n" ,T_fsm,h);
    writeVTKFile("erreur_200_n" ,T_erreur,h);
    
    writeVTKFile("closest_FIM_200_n" ,closest_fim,h);
    writeVTKFile("closest_FSM_200_n" ,closest_fsm,h);
    writeVTKFile("closest_erreur_200_n" ,closest_erreur,h);
    
    writeMatrixToFile(T_fim,"/results/FIM_result.txt") ;
    writeMatrixToFile(T_fsm,"/results/FSM_result.txt") ;
    writeMatrixToFile(T_exact,"/results/exact_result.txt") ;

    return 0;
}
