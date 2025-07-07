#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <fstream>
#include <chrono>
#include <random>
#include <tuple>
#include "solveEikonal3D.cpp"
#include "sweep3D.cpp"
#include "print.cpp"
#include <omp.h>



// Fonction pour calculer le minimum entre deux vecteurs 3D
void minVec3D(std::vector<std::vector<std::vector<double>>>& out, const std::vector<std::vector<std::vector<double>>>& in) {
    for (size_t i = 0; i < out.size(); ++i) {
        for (size_t j = 0; j < out[i].size(); ++j) {
            for (size_t k = 0; k < out[i][j].size(); ++k) {
                out[i][j][k] = std::min(out[i][j][k], in[i][j][k]);
            }
        }
    }
}

// Déclaration de la réduction personnalisée pour std::vector<std::vector<std::vector<double>>>
#pragma omp declare reduction(minVec3D : std::vector<std::vector<std::vector<double>>> : minVec3D(omp_out, omp_in)) \
    initializer(omp_priv = omp_orig)



/**
 * @brief fonction pour appliquer la Fast Sweeping Methode(FSM)
 *
 * Cette méthode ce base sur des balayements du maillage dans les directions cardinales selon le nombre de dimension
 * On va parcourir les maillages dans chaques directions et résoudre le problème eikonal à chaque point ce qui fais 
 * que l'on va itèrer sur n points fois le nombre de balayage. 
 * 
 * -T :Matrice qui stocke les distances
 * -n : nombre de point selon x et y
 * -lenght : longueur de l'axe x et y
 * -Xs : vecteur contenant l'indice des sources
 * -Closest : Matrice contenant le tag de la source la plus proche
 
 */
double FSM3D(std::vector<std::vector<std::vector<double>>>& T, int n, double length, const std::vector<std::tuple<int, int, int>>& Xs, std::vector<std::vector<std::vector<double>>>& Closest) {
    auto start_t = std::chrono::high_resolution_clock::now();
    double compteur_Xs = 1;
    //initialisation de T
    for (auto xi : Xs) {
        int i = std::get<0>(xi);
        int j = std::get<1>(xi);
        int k = std::get<2>(xi);
        T[i][j][k] = 0;
        Closest[i][j][k] = compteur_Xs;
        compteur_Xs++;
    }

    int m = n;
    double h = length / (n - 1);
    int a ; 
    std::vector<std::vector<std::vector<double>>> F(n, std::vector<std::vector<double>>(n, std::vector<double>(n, 1.0)));
    int N = 3;
    std::vector<std::tuple<int, int, int>> direction = { {-1, -1, -1}, {1, -1, -1}, {-1, 1, -1}, {1, 1, -1},
                                                         {-1, -1, 1 }, {1, -1, 1 }, {-1, 1,  1}, {1, 1, 1} };

    omp_set_num_threads(8);
    // boucle parallèle sur T (Résultat faux pour le moment !!!!!!!)
    // le premier for est a remplacer pas un while converge de tout les points mais pour le moment probleme de convergence 
    bool global_stop = false;
    bool test1,test2,test3,test4,test5,test6,test7,test8;
    while(!global_stop){
        
    #pragma omp parallel for schedule(static, 1) reduction(minVec3D : T)
    for (int j = 0; j < 8; j++) {
        int* sweep_direction = new int[3]{ std::get<0>(direction[j]), std::get<1>(direction[j]), std::get<2>(direction[j]) };

        switch (j) {
            case 0:
                test1 = Sweep(T, Closest, F, sweep_direction, N, n, m, h);
                break;
            case 1:
                test2 = Sweep(T, F, Closest, sweep_direction, N, n, m, h);
                break;
            case 2:
                test3 = Sweep(T, F, Closest, sweep_direction, N, n, m, h);
                break;
            case 3:
                test4 = Sweep(T, F, Closest, sweep_direction, N, n, m, h);
                break;
            case 4:
                test5 = Sweep(T, F, Closest, sweep_direction, N, n, m, h);
                break;
            case 5:
                test6 = Sweep(T, F, Closest, sweep_direction, N, n, m, h);
                break;
            case 6:
                test7 = Sweep(T, F, Closest, sweep_direction, N, n, m, h);
                break;
            case 7:
                test8 = Sweep(T, F, Closest, sweep_direction, N, n, m, h);
                break;
        }

        //std::cin >> a ;
        delete[] sweep_direction; // Éviter les fuites mémoire
    }
    if (test1 == true && test2 == true  && test3 == true  && test4 == true && test5 == true && test6 == true  && test7 == true  && test8 == true  ){
        global_stop = true;
    }
}
    
   
    
    auto end_t = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_t - start_t;
    std::cout << "Temps d'exécution FSM parallèle : " << duration.count() << " secondes. " << " threads " << std::endl;

    return duration.count();
}
