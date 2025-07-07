#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <cmath>
#include <sstream>  
#include <fstream>
#include <chrono>

#pragma once 

void print(const std::vector<std::vector<double>>& T2D)
{
    std::cout << " T = " << std::endl ;
    for(auto raw : T2D)
       { for (auto columns : raw)
        std::cout << columns  << " ";
    std::cout << std::endl;}
}


/**
 * @brief Fonction visant à être utilisés dans le solve Eikonal
 * Cette fonction regarde les voisins du points au coordonées xi dans T2D sur l'axe x ET y, sur chaque axe 
 * on va chercher le points qui a la valeur minimum. Elle va en plus données l'indice du point le plus proche de xi parmi les voisins dans closest
 * La fonction retourne un entier a qui correspond au nombre d'élément différends dans liste.Il se peut que l'on ajoute rien dans liste, si tous les voisins ont
 * une valeur infinie (std::numeric_limits<double>::infinity())
 * 
 * -T2D : matrice contenant les distances 
 * -xi : indice (i,j) du point dont l'on veut les voisins 
 * -liste: stock les valeurs de T2D des voisins de xi (un par axe, le plus proche) 
 * -closest : stock les indices de T2D des points de liste.
 * -n : taille de T2D
 * 
 */
int findMinValues2D(const std::vector<std::vector<double>>& T2D, const int *xi, int n, std::vector<double>& liste,std::vector<std::pair<int,int>> &closest) {
    double  min_t, min_t1, min_t2;
    int a  = 2 ;
    std::pair<int,int> ind;
    
    for(int axis = 0  ; axis < 2 ; axis++)
            {   min_t1 = std::numeric_limits<double>::infinity();
                min_t2 = std::numeric_limits<double>::infinity();
                if (axis == 0)
                {
                    if (xi[1] == 0) {
                        min_t = T2D[xi[0]][xi[1]+1];
                        ind = {xi[0],xi[1]+1};
                    }
                    else if (xi[1] == n-1) {
                        min_t = T2D[xi[0]][xi[1]-1];
                        ind = {xi[0],xi[1]-1};
                    }
                    else
                    {
                        min_t1 = T2D[xi[0]][xi[1]-1];
                        min_t2 = T2D[xi[0]][xi[1]+1] ;
                        //std::cout << "min_t1 == " << min_t1 << "min_t2 = " << min_t2 << std::endl; 
        
                        if (min_t1 > min_t2) {
                            min_t = min_t2;
                            ind = {xi[0],xi[1]+1};
                        }
                        else {
                            min_t = min_t1;
                            ind = {xi[0],xi[1]-1};
                        }
                    }
                }
        
                else
                {
                    if (xi[0] == 0)
                    {
                        min_t = T2D[xi[0]+1][xi[1]];
                        ind = {xi[0]+1,xi[1]};
        
                    }
                    else if (xi[0] == n-1)
                    {
        
                        min_t = T2D[xi[0]-1][xi[1]];
                        ind = {xi[0]-1,xi[1]};
                    }
                    else {
                        min_t1 = T2D[xi[0]-1][xi[1]];
                        min_t2 = T2D[xi[0]+1][xi[1]] ;
        
                        if (min_t1 > min_t2)
                        {   min_t = min_t2;
                            ind = {xi[0]+1,xi[1]};
                        }
                        else
                        {   min_t = min_t1;
                            ind = {xi[0]-1,xi[1]};
                        }
                    }
                }
                if ((min_t < std::numeric_limits<double>::infinity()) && (min_t <T2D[xi[0]][xi[1]]))
                    {
                    liste.push_back(min_t);
                }
                else
                    a--;
                closest.push_back(ind);
            }
    return a;
}


/**
 * @brief Résouds le problème quadratique associé à l'équation eikonale 
 * 
 * -xi : point où l'on résouds notre problème 
 * -dim : nombre de point dans liste 
 * -liste : liste des distances des voisins 
 * -F : vitesse associés au points xi 
 * -h : le pas selon x et y  
 */

double SolveNdim2D(const int *xi,int dim,const std::vector<double> &liste,const std::vector<std::vector<double>> &F,double h)
{   double f;
    f = F[xi[0]][xi[1]];


    if (dim == 0) {
        return liste[0]+ h/f;
    }
    
    double sumT = 0;
    double sumT2 = 0 ;
    
    for (double val : liste )
    {
        sumT += val;
        sumT2 += val*val;
    }

    double a = dim+1;
    double b = -2*sumT;
    double c = sumT2 - h*h / f;

    double q = b*b - 4*a*c;

    if (q < 0)
        return std::numeric_limits<double>::infinity();
        else{
        
            double T_new = (-b + std::sqrt(q) )/ (2 * a) ;
            auto ind = std::max_element(liste.begin(),liste.end());
            if(T_new  <= *ind)
                {
                T_new = *std::min_element(liste.begin(),liste.end()) + h/f;
                }
            
    
    return T_new;
}}


/**
 * @brief résous l'équation Eikonal 2d et 3d 
 * 
 * xi : point on grid, is a liste of len(2)
 * n  : number of point in x and y
 * h  : steps delta_x
 * F  : double List of speed associated to each point
 * T  : double List of distance associated to each point 
 */

double SolveEikonale2D(const int *xi, const std::vector<std::vector<double>> &T,const std::vector<std::vector<double>> &F,int n,double h,std::pair<int,int> &closest)
{
    double inf = std::numeric_limits<double>::infinity();
    std::vector<double> liste;
    int a ;
    std::vector<std::pair<int,int>> close;
    //std::cout << T[xi[0]][xi[1]] << std::endl;  
    a = findMinValues2D(T,xi,n,liste,close); // doit donnée 0 ,1 , ou 2 

    if (a == 0) {
        closest = {xi[0],xi[1]};
        return inf;
    }   
        
    std::sort(liste.begin(), liste.end());
    double ti_tild = 0;

    // on boucle sur combiens de dimension ont donné une distance finies,
    for(int dim = 0 ; dim < a; dim++) {
        ti_tild = SolveNdim2D(xi,dim,liste,F,h);

        if ((dim == a-1) or (ti_tild < liste[dim+1]))
            break ;
    }

    auto ind_1 = close[0];
    auto ind_2 = close[1];
    double dist1 = T[std::get<0>(ind_1)][std::get<1>(ind_1)];
    double dist2 = T[std::get<0>(ind_2)][std::get<1>(ind_2)];

    if(dist1> dist2) {

        closest = {std::get<0>(ind_2),std::get<1>(ind_2)};

    }
    else if (dist1 < dist2)  {

        closest = {std::get<0>(ind_1),std::get<1>(ind_1)};
    }
    else {

        closest = {std::get<0>(ind_1),std::get<1>(ind_1)};

    }

    return ti_tild;
      

}
