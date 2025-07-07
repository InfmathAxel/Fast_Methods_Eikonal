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
#include "include/FIM_new.cpp"
#include "include/FSM.cpp"
#include "include/random_source3D.cpp"



/**
 * Main pour testes les methodes FIM,FSM et eucli en fonction du nombre de point n sur les axes x et y (mesh n*n)
 * de longueur lenght et de pas h, si on veut generer des sources aléatoires on indique le n_sources, sinon
 * on utilises generate_source_sinus qui genere un onde sur toutes la largeur du maillages.
 */

 int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <n_source> <n> <n_threads" << std::endl;
        return 1;
    }

    int n_source = std::stoi(argv[1]);
    int n = std::stoi(argv[2]);
    int n_threads = std::stoi(argv[3]);
    
    double length = 10;
    
    
    
    
    std::vector<std::vector<double>> T_FIM(n, std::vector<double>(n, std::numeric_limits<double>::infinity()));
    std::vector<std::vector<double>> T_FSM(n, std::vector<double>(n, std::numeric_limits<double>::infinity()));
    std::vector<std::vector<double>> T_euc(n, std::vector<double>(n, std::numeric_limits<double>::infinity()));

    
    std::vector<std::vector<double>> T_FIM2(n, std::vector<double>(n, std::numeric_limits<double>::infinity()));

    std::vector<std::vector<double>> closest_FIM(n, std::vector<double>(n, 0));
    std::vector<std::vector<double>> closest_FSM(n, std::vector<double>(n, 0));
    std::vector<std::vector<double>> closest_euc(n, std::vector<double>(n, 0));
    std::vector<std::vector<double>> F(n, std::vector<double>(n, 1.0));
    std::vector<std::pair<int,int>> Xs;
    
    double seuil = 2 ;

    int seed = 12345;
    bool give_seed = true;
    //generate_source2d(Xs,n,n_source,seed,give_seed);
    generate_source_sinus2d(Xs,n,seed,give_seed);
    generate_source_sinus2d(Xs,n,2*seed,give_seed);
   
    std::cout << "Xs generer " << std::endl;
    std::cout << "Pour n points : " << n << " Et n_source :" << n_source << std::endl;
    
    
    //FSM2D(T_FSM,F,n,length,Xs,4);
    //FIM2D(T_FIM,F,n,length,Xs,n_threads,closest_FIM,true);  
    //euclidien2d(T_euc,Xs,n,n,length,n_threads,closest_euc);
    FSM2D_seuil(T_FSM,F,n,length,seuil,Xs,4);
    FIM2D_new(T_FIM,F,n,length,seuil,Xs,n_threads,closest_FIM,false);  
    //FIM2D(T_FIM,F,n,length,Xs,n_threads,closest_FIM,false);
    euclidien2d_seuil(T_euc,Xs,n,n,length,seuil,n_threads,closest_euc);
 
    std::string tag_fim = "results/length_10_n_4000_n_source_4FIM";
    std::string tag_closest_fim = "Closest_fim";
    std::string tag_fsm = "results/length_10_n_4000_n_source_4FSM";
    std::string tag_closest_fsm = "Closest_fsm";
    std::string tag_euc = "results/length_10_n_4000_n_source_4eucl";
    std::string tag_closest_euc = "closest_euc.txt";

    std::cout << "Ecriture des fichiers : " << std::endl;
    write_files(T_FIM,tag_fim);
    write_files(T_FSM,tag_fsm);
    write_files(T_euc,tag_euc);
    save_Xs(Xs,"coords_mesh.txt",length/(n-1));
    //write_files(closest_euc,"closest_euc.txt");
    //write_files(closest_FIM,tag_fim);
    //write_files(closest_FSM,tag_fsm);
    
    
    return 0;
}