#include <vector>
#include <iostream>
#include "maille.hpp"
#pragma once 

void print_mat(std::vector<std::vector<double>> &Closest){
    
    for (auto &raw : Closest){
        for(auto &col : raw){
            std::cout << col << " " ; 

        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

double eucli(const coords &point_1 ,const coords &point_2){
    double dx = point_1.get_x() - point_2.get_x();
    double dy = point_1.get_y() - point_2.get_y();
    return std::sqrt(dx * dx + dy * dy);
}