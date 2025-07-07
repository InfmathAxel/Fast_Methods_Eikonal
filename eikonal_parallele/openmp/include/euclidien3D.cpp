#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <optional>
#pragma once 

void linspace2(std::vector<double> &X,double length,int n)
/** 
*  @brief équivalent a  X = np.linspace(0,length,n);
* - X vecteur qui stock les point 
* - lenght longueur selon un axe 
* - n nombre de point 
*/
{
    double step = length/(n-1);
    for(int i =0 ; i < n ; i ++)
    {   
        X.push_back(i*step);
    }

}

double euclidien3D( std::vector<std::vector<std::vector<double>>> &T_exact,std::vector<std::tuple<int,int,int>> Xs ,int n ,int m,double length,std::vector<std::vector<std::vector<double>>> &Closest_exact,int n_thread  )
/** 
*   @brief Fonction de calcul de solution exacte.
*
* - T_exact matrice qui stocke les distances du point (i,j) dans T_exact[i][j]
* - Xs la liste des sources 
* - n,m liste des points dans x,y
* - length longueur et largeur du maillage
*/
{
    std::vector<double> X;
    std::vector<double> Y;
    std::vector<double> Z;
    linspace2(X,length,n);
    linspace2(Y,length,n);
    linspace2(Z,length,n);
    auto start_t = std::chrono::high_resolution_clock::now();
    
    #pragma omp parallel for collapse(3) schedule(dynamic)
    for(int i = 0 ; i < n ; i ++)
    {
        for(int j = 0 ; j < m ; j ++)
        {   
            for(int k = 0 ; k < n ; k++)
            {
            double dist = 1e5;
            int compteur_xs  = 1;
            int indice_closest = 0 ;
            for (auto xi : Xs)
                {   
                int x_ind = std::get<0>(xi);
                int y_ind = std::get<1>(xi);
                int z_ind = std::get<2>(xi);
                double x = X[x_ind];
                double y = Y[y_ind];
                double z = Z[z_ind];
                double eucli = std::sqrt(pow(x-X[i],2)+ std::pow(y-Y[j],2)+std::pow(z-Z[k],2));
                //std::cout << x << " " << y << " " << eucli << std::endl;
                if (eucli < dist)
                    {dist = eucli;
                    indice_closest =  compteur_xs;
                    }
                compteur_xs ++;
            }
            T_exact[i][j][k] = dist;
            Closest_exact[i][j][k] = indice_closest;
            }   
        }
    }
    auto end_t = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_t - start_t;

    // Afficher le temps écoulé
    std::cout << "Temps d'exécution euclidien : " << duration.count() << " secondes." << std::endl;
    return duration.count();
}


void erreur(std::vector<std::vector<std::vector<double>>> &T_erreur, const std::vector<std::vector<std::vector<double>>> &T_exact, const std::vector<std::vector<std::vector<double>>> &T){
    int n = T_exact.size();
    
    #pragma omp parallel for collapse(3) schedule(dynamic)
    for(int i = 0 ; i < n ; i ++)
    {
        for(int j = 0 ; j < n ; j ++)
        {   
            for(int k = 0 ; k < n ; k++)
            {
                T_erreur[i][j][k] = std::abs(T_exact[i][j][k] - T[i][j][k]); 
            }
        }
    }
}

void erreur_closest(std::vector<std::vector<std::vector<double>>> &T_erreur, const std::vector<std::vector<std::vector<double>>> &T_exact, const std::vector<std::vector<std::vector<double>>> &T){
    int n = T_exact.size();
    
    #pragma omp parallel for collapse(3) schedule(dynamic)
    for(int i = 0 ; i < n ; i ++)
    {
        for(int j = 0 ; j < n ; j ++)
        {   
            for(int k = 0 ; k < n ; k++)
            {
               double error = std::abs(T_exact[i][j][k] - T[i][j][k]); 
               if (error == 0){
                T_erreur[i][j][k] = 0;
               }
               else 
               T_erreur[i][j][k] = 1;
            }
        }
    }
}