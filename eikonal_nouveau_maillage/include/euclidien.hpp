#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>
#include "source.hpp"
#include "maille.hpp"
#pragma once 

void linspace(std::vector<double> &X,double lenght,int n){
/** 
*  @brief équivalent a  X = np.linspace(0,lenght,n);
* - X vecteur qui stock les point 
* - lenght longueur selon un axe 
* - n nombre de point 
*/
    double step = lenght/(n-1);
    for(int i =0 ; i < n ; i ++)
    {   
        X.push_back(i*step);
    }

}

double euclidien( std::vector<std::vector<double>> &T_exact,std::vector <source> Xs ,int n ,int m,double lenght )
/** 
*   @brief Fonction de calcul de solution exacte.
*
* - T_exact matrice qui stocke les distances du point (i,j) dans T_exact[i][j]
* - Xs la liste des sources 
* - n,m liste des points dans x,y
* - lenght longueur et largeur du maillage
*/
{
    std::vector<double> X;
    std::vector<double> Y;
    linspace(X,lenght,n);
    linspace(Y,lenght,n);
    auto start_t = std::chrono::high_resolution_clock::now();
    for(int i = 0 ; i < n ; i ++)
    {
        for(int j = 0 ; j < m ; j ++)
        {   double dist = 1e5;
            for (auto xi : Xs)
            {  
                double x = xi.get_x();
                double y = xi.get_y();

                double eucli = std::sqrt(pow(x-X[i],2)+ std::pow(y-Y[j],2));
                //std::cout << x << " " << y << " " << eucli << std::endl;
                if (eucli < dist)
                    dist = eucli;

            }
            T_exact[i][j] = dist;
        }
    }
    auto end_t = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_t - start_t;

    // Afficher le temps écoulé
    std::cout << "Temps d'exécution euclidien : " << duration.count() << " secondes." << std::endl;
    return duration.count();
}
