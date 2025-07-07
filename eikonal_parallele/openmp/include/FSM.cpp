#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <cmath>
#include <sstream>  // Pour construire la chaîne formatée
#include <fstream>
#include <chrono>
#include <random>
#include "solveEikonal.cpp"
#include "sweep.cpp"
#include "write_result.cpp"
#include "random_source.cpp"
#include <omp.h>

#pragma once 
void print_t(std::vector<std::vector<double>> &Closest) {

    for (auto &raw : Closest) {
        for(auto &col : raw) {
            std::cout << col << " " ;

        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void minVec(std::vector<std::vector<double>>& out, const std::vector<std::vector<double>>& in) {
    for (size_t i = 0; i < out.size(); ++i) {
        for (size_t j = 0; j < out[i].size(); ++j) {
            out[i][j] = std::min(out[i][j], in[i][j]);
        }
    }
}

// Déclaration de la réduction personnalisée pour std::vector<std::vector<double>>
#pragma omp declare reduction(minVec : std::vector<std::vector<double>> : minVec(omp_out, omp_in)) \
    initializer(omp_priv = omp_orig)


    
double FSM2D(std::vector<std::vector<double>> &T, const std::vector<std::vector<double>> &F, int n, double length, const std::vector<std::pair<int, int>> &Xs,int num_threads) {
    auto start_t = std::chrono::high_resolution_clock::now();
    double h = length/(n-1);
    for (auto &xi : Xs) {
        int i = std::get<0>(xi);
        int j = std::get<1>(xi);
        T[i][j] = 0;
    }
    int m = n;
    
    int N = 2 ;
    bool stop = false;
    bool test1,test2,test3,test4;
    int compteur = 0 ;
    std::vector<std::pair<int, int>> direction = {{1,1}, {1,-1}, {-1,1}, {-1,-1}}; 
    //for (int i = 0 ; i < 3 ; i++)
    omp_set_num_threads(num_threads);
    while(!stop)
    {
    #pragma omp parallel for schedule(static,1) reduction(minVec : T)// chaque thread ne fais qu'une operation , chaque thread fais dans la direction voulu 
    for (int j = 0; j < 4; j++)  
    {   
        int* sweep_direction = new int[2]{std::get<0>(direction[j]), std::get<1>(direction[j])};

        switch (j)
        {
            case 0:
                test1 = Sweep(T, F, sweep_direction, N, n, m, h);
                break;
            case 1:
                
                test2 = Sweep(T, F, sweep_direction, N, n, m, h);
                
                break;
            case 2:
                
                test3 = Sweep(T, F, sweep_direction, N, n, m, h);
                
                break;
            case 3:
                test4 = Sweep(T, F, sweep_direction, N, n, m, h);
                break;
        }

       


        delete[] sweep_direction; // Éviter les fuites mémoire
    }
    if (test1 == true && test2 == true  && test3 == true  && test4 == true ){
        stop = true;
    }
    compteur ++;
}

    std::cout << "nb de sweep : " << compteur *4 << std::endl;
    auto end_t = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_t - start_t;
    std::cout << "Temps d'exécution FSM parallèle : " << duration.count() << " secondes. " << " threads " << std::endl;

    return duration.count() ; 
}



    
double FSM2D_seuil(std::vector<std::vector<double>> &T, const std::vector<std::vector<double>> &F, int n, double length,double seuil, const std::vector<std::pair<int, int>> &Xs,int num_threads) {
    auto start_t = std::chrono::high_resolution_clock::now();
    double h = length/(n-1);
    for (auto &xi : Xs) {
        int i = std::get<0>(xi);
        int j = std::get<1>(xi);
        T[i][j] = 0;
    }
    int m = n;
    
    int N = 2 ;
    bool stop = false;
    bool test1,test2,test3,test4;
    int compteur = 0 ;
    std::vector<std::pair<int, int>> direction = {{1,1}, {1,-1}, {-1,1}, {-1,-1}}; 
    //for (int i = 0 ; i < 3 ; i++)
    omp_set_num_threads(num_threads);
    while(!stop)
    {
    #pragma omp parallel for schedule(static,1) reduction(minVec : T)// chaque thread ne fais qu'une operation , chaque thread fais dans la direction voulu 
    for (int j = 0; j < 4; j++)  
    {   
        int* sweep_direction = new int[2]{std::get<0>(direction[j]), std::get<1>(direction[j])};

        switch (j)
        {
            case 0:
                test1 = Sweep_seuil(T, F, sweep_direction, seuil,N, n, m, h);
                break;
            case 1:
                
                test2 = Sweep_seuil(T, F, sweep_direction, seuil,N, n, m, h);
                
                break;
            case 2:
                
                test3 = Sweep_seuil(T, F, sweep_direction, seuil,N, n, m, h);
                
                break;
            case 3:
                test4 = Sweep_seuil(T, F, sweep_direction, seuil,N, n, m, h);
                break;
        }

       


        delete[] sweep_direction; // Éviter les fuites mémoire
    }
    if (test1 == true && test2 == true  && test3 == true  && test4 == true ){
        stop = true;
    }
    compteur ++;
}

    std::cout << "nb de sweep : " << compteur *4 << std::endl;
    auto end_t = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_t - start_t;
    std::cout << "Temps d'exécution FSM parallèle : " << duration.count() << " secondes. " << " threads " << std::endl;

    return duration.count() ; 
}