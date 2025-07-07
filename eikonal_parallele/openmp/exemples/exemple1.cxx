#include "../include/solveEikonal.cpp"
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
#include "../include/write_result.cpp"
#include "../include/random_source.cpp"
#include "../include/euclidien.cpp"
#include <omp.h>
#include "../include/FIM.cpp"
#include "../include/FSM.cpp"
#include "../include/random_source3D.cpp"

int main()
{
    //initialisation des données 
    int n = 400;
    int nb_source = 4;
    double length = 10;
    double h = length/(n-1);
    int n_thread = 1;
    //initialisation matrice de distance pour la FIM et FSM

    std::vector<std::vector<double>> T_FSM(n, std::vector<double>(n, std::numeric_limits<double>::infinity()));
    std::vector<std::vector<double>> T_FIM(n, std::vector<double>(n, std::numeric_limits<double>::infinity()));
    std::vector<std::vector<double>> closest_FSM(n,std::vector<double>(n,0));
    std::vector<std::vector<double>> closest_FIM(n,std::vector<double>(n,0));
    
    //Initialisation de la matrice de vitesse
    std::vector<std::vector<double>> F(n, std::vector<double>(n, 1));

    //initialisation des Xs a la main
    std::vector<std::pair<int,int>> Xs;
    Xs.push_back({100,100});
    Xs.push_back({100,300});
    Xs.push_back({300,100});
    Xs.push_back({300,300});
    
    //Appel de la FIM et FSM

    FIM2D(T_FIM,F,n,h,Xs,n_thread,closest_FIM);
    FSM2D(T_FSM,F,n,h,Xs,n_thread);

    //écriture des fichiers résultats

    write_files(T_FIM,"resultat_exemple1/FIM_exemple1");
    write_files(T_FIM,"resultat_exemple1/FSM_exemple1");
    save_Xs(Xs,"resultat_exemple1/Xs",length/(n-1));

    std::cout << "On a généré deux fichiers txt contenant les résultats. On peut maintenant faire la visualisation avec le code python 'visu_exemple1.py' "  << std::endl;
    return 0;
}