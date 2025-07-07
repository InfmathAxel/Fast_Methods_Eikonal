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

#pragma once 


double FIM2D_new(std::vector<std::vector<double>> &T, const std::vector<std::vector<double>> &F, int n, double length,double seuil, const std::vector<std::pair<int, int>> &Xs, int n_threads,std::vector<std::vector<double>> &closest_fim,bool verbose = false) {
    auto start_t = std::chrono::high_resolution_clock::now();
    double h = length/(n-1);
    int compteur_xs = 1;
    int m = n ;
    omp_set_num_threads(n_threads);
    double eps = 1e-5;
    int tag_iter = 0 ;
    std::string tag = "FIM";

    if (verbose == true ){

        std::string dirPath = "/ccc/work/cont001/ocre/demutha/results/" + tag;
        struct stat info;
        if (stat(dirPath.c_str(), &info) != 0) {
            if (mkdir(dirPath.c_str(), 0755) != 0) {
                std::cerr << "Erreur : Impossible de créer le dossier " << dirPath << std::endl;
            return 1;
        }
        } else if (!(info.st_mode & S_IFDIR)) {
            std::cerr << "Erreur : " << dirPath << " existe mais n'est pas un dossier." << std::endl;
            return 1 ;
        } else {
            // Vider le dossier s'il existe déjà
            clearDirectory(dirPath);
            std::cout << "dossier clear" << std::endl ;
        }
        }
    int nb_source_by_thread = Xs.size()/n_threads;
    
   #pragma omp parallel
    {
        #pragma omp single
        {
            for (int idx = 0; idx < n_threads; ++idx) 
            {
                #pragma omp task firstprivate(idx)
                {   
            {   std::vector<std::pair<int, int>> direction = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
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

                    
                   
                        
                    if (verbose == true)
                    {
                    #pragma critical
                    {
                    tag_iter++;
                    writeVTKFile2D(tag,T,h,tag_iter);
                    }
                    }
                }
                }
            }
        }
    }
        
    if (verbose == true)
                    {
                    #pragma critical
                    {
                    tag_iter++;
                    writeVTKFile2D(tag,T,h,tag_iter);
            }
        }
    auto end_t = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_t - start_t;
    std::cout << "Temps d'exécution FIM parallèle : " << duration.count() << " secondes. " << n_threads << " threads " << std::endl;

    return duration.count();
    }

    