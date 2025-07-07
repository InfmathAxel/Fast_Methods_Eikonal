#include "solveEikonal.cpp"
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
#include "write_result.cpp"
#include "random_source.cpp"
#include "euclidien.cpp"
#include "solveEikonal3D.cpp"
#include "print.cpp"
#include <omp.h>

// Structure de hachage pour std::tuple<int, int, int>
template <>
struct std::hash<std::tuple<int, int, int>> {
    size_t operator()(const std::tuple<int, int, int>& triplet) const {
        return std::hash<int>()(std::get<0>(triplet)) ^
               std::hash<int>()(std::get<1>(triplet)) ^
               std::hash<int>()(std::get<2>(triplet));
    }
};

// Fonction d'égalité pour std::tuple<int, int, int>
struct tuple_equal {
    bool operator()(const std::tuple<int, int, int>& lhs, const std::tuple<int, int, int>& rhs) const {
        return std::get<0>(lhs) == std::get<0>(rhs) &&
               std::get<1>(lhs) == std::get<1>(rhs) &&
               std::get<2>(lhs) == std::get<2>(rhs);
    }
};


/**
 * @brief fonction pour appliquer la Fast Iterative Methode(FIM)
 *
 *  Cet méthode ce base sur le parcours d'une bande active ou l'on stocke les voisins
 *  des points sur lesquels nous avons déjà travaillé et des interfaces  pour être sur de pouvoir avancer vers la convergence
 *  du calcule de distance en ces points et ne pas repassé de trop nombreuse fois sur eux en faisant des calculs inutiles
 *
 * -T :Matrice qui stocke les distances
 * -n : nombre de point selon x et y
 * -lenght : longueur de l'axe x et y
 * -Xs : vecteur contenant l'indice des sources
 * -Closest : Matrice contenant le tag de la source la plus proche
 */
double FIM3D(std::vector<std::vector<std::vector<double>>>& T, const std::vector<std::vector<std::vector<double>>>& F, int n, double length, const std::vector<std::tuple<int, int, int>>& Xs, int n_threads,std::vector<std::vector<std::vector<double>>>& closest) {
    auto start_t = std::chrono::high_resolution_clock::now();
    double h = length/(n-1);
    int m = n;
    std::array<int, 3> close;
    int tag = 1;
    //initialisation de T
    for (auto& xi : Xs) {
        int i = std::get<0>(xi);
        int j = std::get<1>(xi);
        int k = std::get<2>(xi);
        T[i][j][k] = 0;
        closest[i][j][k] = tag;
        tag ++;

    }
    
    int seuil = 4 ;
    // fixe le nombre de threads 
    omp_set_num_threads(n_threads);
    double eps = 1e-5;

    //initialisations des tableau pour compter les itérations 
    int tab[Xs.size()];
    int tab2[Xs.size()];


    //Debut de la section parallèle, ici on parallelise par rapport au source, chaque threads prends un point de Xs et applique la FIM en initialisant la bande
    // active à partir de ce point 
    //Améliorations possible: donner a chaque threads X source plutôt que une a chaque fois, rentre plus safe avec un T local qui update de temps en temps le T 
    // global ? 

    #pragma omp parallel
    {   
        #pragma omp single
        {  
            for (int idx = 0; idx < Xs.size(); ++idx) {
                #pragma omp task firstprivate(idx)
                {   
                    {
                    auto xi = Xs[idx];
                    int v = std::get<0>(xi);
                    int w = std::get<1>(xi);
                    int z = std::get<2>(xi);
                    std::unordered_set<std::tuple<int, int, int>, std::hash<std::tuple<int, int, int>>, tuple_equal> narrow;
                    std::vector<std::tuple<int, int, int>> direction = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {-1, 0, 0}, {0, -1, 0}, {0, 0, -1}};
                    for (auto& direc : direction) {
                        int di = std::get<0>(direc) + std::get<0>(xi);
                        int dj = std::get<1>(direc) + std::get<1>(xi);
                        int dk = std::get<2>(direc) + std::get<2>(xi);

                        if ((0 <= di) && (di < n) && (0 <= dj) && (dj < m) && (0 <= dk) && (dk < n)) {
                            if (narrow.find({di, dj, dk}) == narrow.end()){
                                narrow.insert({di, dj, dk});
                                closest[di][dj][dk] = closest[std::get<0>(xi)][std::get<1>(xi)][std::get<2>(xi)];
                            }
                        }
                    }
                    // initialisation des compteurs;
                    int iter = 0;
                    int compteur = 0 ;
                    int nb_point = 0 ;
                    // debut de la boucle de la fim sur le narrow local

                    while (!narrow.empty() && iter < 100000) {

                        std::vector<std::tuple<int, int, int>> copie_narrow_vec(narrow.begin(), narrow.end());
                        std::unordered_set<std::tuple<int, int, int>, std::hash<std::tuple<int, int, int>>, tuple_equal> copie_narrow(narrow);
                        iter++;
                        compteur++;
                        
                        
                        for (const auto& pair : copie_narrow_vec) {
                            int i = std::get<0>(pair);
                            int j = std::get<1>(pair);
                            int k = std::get<2>(pair);
                            int xi[3] = {i, j, k};
                            
                            double ti = T[i][j][k];
                            double T_tild = SolveEikonale3D(xi, T, F, n, h, close);
                            
                            T[i][j][k] = T_tild;
                            closest[i][j][k] = closest[close[0]][close[1]][close[2]];
                            

                            std::vector<std::tuple<int, int, int>> neighboor;
                            for (auto& direc : direction) {
                                int di = std::get<0>(direc) + xi[0];
                                int dj = std::get<1>(direc) + xi[1];
                                int dk = std::get<2>(direc) + xi[2];

                                if ((0 <= di) && (di < n) && (0 <= dj) && (dj < m) && (0 <= dk) && (dk < n)) {
                                    neighboor.push_back({di, dj, dk});
                                }
                            }

                            if ((std::abs(ti - T_tild) < eps)){ //|| (T_tild < seuil)) {
                                for (const auto& pair2 : neighboor) {
                                    if (copie_narrow.find(pair2) == copie_narrow.end()) {
                                        int i2 = std::get<0>(pair2);
                                        int j2 = std::get<1>(pair2);
                                        int k2 = std::get<2>(pair2);
                                        int xj[3] = {i2, j2, k2};
                                        
                                        
                                        double ti2 = T[i2][j2][k2];
                                        double T_tild2 = SolveEikonale3D(xj, T, F, n, h, close);
                                        
                                        if (ti2 > T_tild2) {
                                            
                                            T[i2][j2][k2] = T_tild2;
                                            closest[i2][j2][k2] = closest[xi[0]][xi[1]][xi[2]];
                                            narrow.insert({i2, j2, k2});
                                            nb_point++;
                                        }
                                    }
                                }
                                narrow.erase({i, j, k});
                            }
                        }
                        
                        
                    }
                tab[idx] = compteur;
                tab2[idx] = nb_point;
                
                }
                }

            }
        }
    }

    auto end_t = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_t - start_t;
    std::cout << "Temps d'exécution FIM parallèle : " << duration.count() << " secondes. " << n_threads << " threads " << std::endl;

    // si on veut afficher les compteurs : 
    for (int i = 0 ; i < Xs.size();i++){

        //std::cout << "nb  iter boucle " << tab[i] << " nb points " <<  tab2[i] << std::endl;
    }
    std::cout << "fin FIM " << std::endl ;
    return duration.count();
}
