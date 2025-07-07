#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <optional>
#pragma once 

void linspace(std::vector<double> &X,double lenght,int n)
/** 
*  @brief équivalent a  X = np.linspace(0,lenght,n);
* - X vecteur qui stock les point 
* - lenght longueur selon un axe 
* - n nombre de point 
*/
{
    double step = lenght/(n-1);
    for(int i =0 ; i < n ; i ++)
    {   
        X.push_back(i*step);
    }

}

double euclidien2d( std::vector<std::vector<double>> &T_exact,std::vector <std::pair<int,int>> Xs ,int n ,int m,double lenght,std::vector<std::vector<double>> &Closest_exact  )
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
    int indice_closest = 0 ;
    for(int i = 0 ; i < n ; i ++)
    {
        for(int j = 0 ; j < m ; j ++)
        {   double dist = 1e5;
            int compteur_xs  = 1;
            for (auto xi : Xs)
            {   
                int x_ind = std::get<0>(xi);
                int y_ind = std::get<1>(xi);
                double x = X[x_ind];
                double y = Y[y_ind];
                double eucli = std::sqrt(pow(x-X[i],2)+ std::pow(y-Y[j],2));
                //std::cout << x << " " << y << " " << eucli << std::endl;
                if (eucli < dist)
                    {dist = eucli;
                    indice_closest =  compteur_xs;
                    }
                compteur_xs ++;
            }
            T_exact[i][j] = dist;
            Closest_exact[i][j] = indice_closest;
        }
    }
    auto end_t = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_t - start_t;

    // Afficher le temps écoulé
    std::cout << "Temps d'exécution euclidien : " << duration.count() << " secondes." << std::endl;
    return duration.count();
}
