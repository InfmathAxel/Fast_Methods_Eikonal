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
#include <omp.h>
#include "FSM.cpp"
#include "FIM.cpp"
#include <map>
#pragma once 

//Optique d'optimiser la réduction en parallele QUAND ON A UN SEUIL, il faudrait repenser solveEikonal pour se faire avec une map.
//L'idée et de ne stocker QUE LES POINTS ou il se passe qq chose,d'ignorer le reste.

void minMap(std::map<std::pair<int, int>, double> & out, std::map<std::pair<int, int>, double> & in) {
    for (const auto& pair :in) {
        if (out.find({pair.first.first,pair.first.second}) == out.end()) 
            out[{pair.first.first,pair.first.second}] = pair.second;
        
        else
            out[{pair.first.first,pair.first.second}] = std::min(out[{pair.first.first,pair.first.second}],pair.second);
    }
}

// Déclaration de la réduction personnalisée pour std::map<std::pair<int, int>, double>
#pragma omp declare reduction(minMap : std::map<std::pair<int, int>, double> : minMap(omp_out, omp_in)) \
    initializer(omp_priv = omp_orig)


double FIM2D_map(std::vector<std::vector<double>> &T, const std::vector<std::vector<double>> &F, int n, double length,double seuil, const std::vector<std::pair<int, int>> &Xs, int n_threads,std::vector<std::vector<double>> &closest_fim,bool verbose = false) {
    auto start_t = std::chrono::high_resolution_clock::now();
    double h = length/(n-1);
    int compteur_xs = 1;
    int m = n ;
    omp_set_num_threads(n_threads);
    double eps = 1e-5;
    int tag_iter = 0 ;
    std::string tag = "FIM";
    std::map<std::pair<int, int>, double> T_map;
    int nb_source_by_thread = Xs.size()/n_threads;

     #pragma omp parallel for schedule(static,1) reduction(minMap : T_map)//
            for (int idx = 0; idx <n_threads; ++idx) 
            {   
                std::vector<std::pair<int, int>> direction = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
                std::unordered_set<std::pair<int, int>, pair_hash> narrow;
                int start = idx * nb_source_by_thread;
                int end = (idx + 1) * nb_source_by_thread;
                 
                // Assurez-vous de ne pas dépasser la taille de Xs
                if (idx == n_threads - 1) {
                end = Xs.size();
                }
                if(verbose){
                #pragma omp critical
                std::cout << "start : " << start << " end " << end << std::endl;
                }
                for (int id = start; id < end; id++)
                {   
                    auto xi = Xs[id];
                    int i = std::get<0>(xi);
                    int j = std::get<1>(xi);
                    T[i][j] = 0;
                    //closest_fim[i][j] = compteur_xs;
                    compteur_xs ++;
                    int v = std::get<0>(xi);
                    int w = std::get<1>(xi);
                    
                    for (auto &direc : direction) {
                        int di = direc.first + std::get<0>(xi);
                        int dj = direc.second + std::get<1>(xi);

                        if ((0 <= di) && (di < n) && (0 <= dj) && (dj < m)) {
                            if ((narrow.find({di, dj}) == narrow.end())){
                                narrow.insert({di, dj});
                                //closest_fim[di][dj] = //closest_fim[v][w];
                            }
                        }
                    }
                }

                    int iter = 0;
                    while (!narrow.empty() && iter < 100000) {
                        std::vector<std::pair<int, int>> copie_narrow_vec(narrow.begin(), narrow.end());
                        std::unordered_set<std::pair<int, int>, pair_hash> copie_narrow(narrow);
                        iter ++;
                        
                        
                        for (const auto &pair : copie_narrow_vec) {
                            int i = std::get<0>(pair);
                            int j = std::get<1>(pair);
                            int xi[2] = {i, j};
                            double ti = T[i][j];
                            double T_tild = SolveEikonale(xi, T, F, n, h);
                            T[i][j] = T_tild;

                            std::vector<std::pair<int, int>> neighboor;
                            for (auto &direc : direction) {
                                int di = direc.first + std::get<0>(pair);
                                int dj = direc.second + std::get<1>(pair);

                                if ((0 <= di) && (di < n) && (0 <= dj) && (dj < m)) {
                                    neighboor.push_back({di, dj});
                                }
                            }

                            if ((std::abs(ti - T_tild) < eps) && (T_tild < seuil)) {
                                for (const auto &pair2 : neighboor) {
                                    if ((copie_narrow.find(pair2) == copie_narrow.end())) {
                                        int i2 = std::get<0>(pair2);
                                        int j2 = std::get<1>(pair2);
                                        int xj[2] = {i2, j2};
                                        double ti2 = T[i2][j2];
                                        double T_tild2 = SolveEikonale(xj, T, F, n, h);
                                        if (ti2 > T_tild2) {
                                            T[i2][j2] = T_tild2;
                                            narrow.insert({i2, j2});
                                        }
                                    }
                                }
                                narrow.erase({i, j});
                            }
                        }
                        
                        
                    }
            }
                    
                   
            
    auto end_t = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_t - start_t;
    std::cout << "Temps d'exécution FIM parallèle : " << duration.count() << " secondes. " << n_threads << " threads " << std::endl;

    return duration.count();
    }