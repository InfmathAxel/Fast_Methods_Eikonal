#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <omp.h>
#pragma once

void linspace(std::vector<double> &X, double length, int n) {
    double step = length / (n - 1);
    for (int i = 0; i < n; i++) {
        X.push_back(i * step);
    }
}

double euclidean_distance(double x1, double y1, double x2, double y2) {
    return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

double euclidien2d(std::vector<std::vector<double>> &T_exact,  const std::vector<std::pair<int, int>> &Xs, int n, int m, double length, int n_threads,const std::vector<std::vector<double>> &closest_exact) {
    std::vector<double> X;
    std::vector<double> Y;
    linspace(X, length, n);
    linspace(Y, length, m);
    auto start_t = std::chrono::high_resolution_clock::now();

    // Initialize T_exact with a large value
    T_exact = std::vector<std::vector<double>>(n, std::vector<double>(m, std::numeric_limits<double>::max()));

    #pragma omp parallel for num_threads(n_threads) collapse(2)
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            double min_distance = std::numeric_limits<double>::max();
            for (const auto &point : Xs) {
                
                double distance = euclidean_distance(X[i], Y[j], X[point.first], Y[point.second]);
                if (distance < min_distance) {
                    min_distance = distance;
                }
            }
            T_exact[i][j] = min_distance;
        }
    }
    auto end_t = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_t - start_t;

    // Afficher le temps écoulé
    std::cout << "Temps d'exécution euclidien : " << duration.count() << " secondes." << std::endl;
    return duration.count();
}


double euclidien2d_seuil(std::vector<std::vector<double>> &T_exact,  const std::vector<std::pair<int, int>> &Xs, int n, int m, double length,double seuil, int n_threads,const std::vector<std::vector<double>> &closest_exact) {
    std::vector<double> X;
    std::vector<double> Y;
    linspace(X, length, n);
    linspace(Y, length, m);
    auto start_t = std::chrono::high_resolution_clock::now();
    
    // Initialize T_exact with a large value
    T_exact = std::vector<std::vector<double>>(n, std::vector<double>(m, std::numeric_limits<double>::max()));

    #pragma omp parallel for num_threads(n_threads) collapse(2)
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            double min_distance = std::numeric_limits<double>::max();
            for (const auto &point : Xs) {
                
                double distance = euclidean_distance(X[i], Y[j], X[point.first], Y[point.second]);
                if (distance < min_distance) {
                    min_distance = distance;
                }
            }
            
            if (min_distance  <= seuil){
                T_exact[i][j] = min_distance;
            }
            else 
            {
                T_exact[i][j] = std::numeric_limits<double>::infinity();
            }
        }
    }
    auto end_t = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_t - start_t;

    // Afficher le temps écoulé
    std::cout << "Temps d'exécution euclidien : " << duration.count() << " secondes." << std::endl;
    return duration.count();
}
