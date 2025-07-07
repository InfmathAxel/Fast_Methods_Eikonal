
#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <cmath>
#include <sstream>  // Pour construire la chaîne formatée
#include <fstream>
#include <unordered_set>
#pragma once
struct pair_hash {
    std::size_t operator()(const std::pair<int, int>& p) const {
        return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
    }
};


void print_t(std::vector<std::vector<double>> &Closest) {
    for (auto &raw : Closest) {
        for(auto &col : raw) {
            std::cout << col << " " ;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


void print_narrow(std::unordered_set<std::pair<int,int>, pair_hash> narrow)
{   std::cout <<" narrow : ";
    for (auto &pair : narrow)
        std::cout << "("<<pair.first <<"," << pair.second << ") ";
    std::cout<< std::endl;
}

void print_frozen(std::unordered_set<std::pair<int,int>, pair_hash> narrow)
{   std::cout <<" frozen : ";
    for (auto &pair : narrow)
        std::cout << "(" << pair.first <<"," << pair.second << ") ";
    std::cout<< std::endl;
}

// Fonction pour afficher la matrice T couche par couche
void afficherMatrice3D(const std::vector<std::vector<std::vector<double>>>& T) {
    for (size_t k = 0; k < T.size(); ++k) {
        std::cout << "Couche " << k << " :" << std::endl;
        for (size_t i = 0; i < T[k].size(); ++i) {
            for (size_t j = 0; j < T[k][i].size(); ++j) {
                std::cout << T[k][i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl; // Séparateur entre les couches
    }
    std::cout << "///////////////////////////////////////////////////////////////////////////////" << std::endl;
}

void printMatrix3D(const std::vector<std::vector<std::vector<double>>>& matrix) {
    for (size_t layer = 0; layer < matrix.size(); ++layer) {
        std::cout << "Layer " << layer + 1 << ":\n";
        for (const auto& row : matrix[layer]) {
            for (const auto& elem : row) {
                if (elem == std::numeric_limits<double>::infinity()) {
                    std::cout << "inf ";
                } else {
                    std::cout << elem << " ";
                }
            }
            std::cout << std::endl;
        }
        std::cout << std::endl; // Séparation entre les couches
    }
    std::cout << "///////////////////////////////////////////////////////////////////////////////" << std::endl;
}