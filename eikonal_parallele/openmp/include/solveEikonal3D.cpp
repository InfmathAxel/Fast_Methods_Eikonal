#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <cmath>
#include <sstream>  
#include <fstream>
#include <chrono>
#include <array>

#pragma once 



/**
 * @brief Fonction visant à être utilisés dans le solve Eikonal
 * Cette fonction regarde les voisins du points au coordonées xi dans T3D sur l'axe x ET y, sur chaque axe 
 * on va chercher le points qui a la valeur minimum. Elle va en plus données l'indice du point le plus proche de xi parmi les voisins dans closest
 * La fonction retourne un entier a qui correspond au nombre d'élément différends dans liste.Il se peut que l'on ajoute rien dans liste, si tous les voisins ont
 * une valeur infinie (std::numeric_limits<double>::infinity())
 * 
 * -T3D : matrice contenant les distances 
 * -xi : indice (i,j,k) du point dont l'on veut les voisins 
 * -liste: stock les valeurs de T3D des voisins de xi (un par axe, le plus proche) 
 * -closest : stock les indices de T3D des points de liste.
 * -n : taille de T3D
 * 
 */
int findMinValues3D(const std::vector<std::vector<std::vector<double>>>&T3D, const int *xi, int n, std::vector<double>& liste,std::vector<std::array<int,3>> &indice) {
    double min_t, min_t1, min_t2;
    int a  = 3 ;
    for(int axis = 0 ; axis < 3 ; axis++)
        {   std::array<int,3> index;
            min_t1 = std::numeric_limits<double>::infinity();
            min_t2 = std::numeric_limits<double>::infinity();
            if (axis == 0)
                {
                if (xi[2] == 0){
                    min_t = T3D[xi[0]][xi[1]][xi[2]+1];
                    index = {xi[0],xi[1],xi[2]+1};
                }
                else if (xi[2] == n-1){

                    min_t = T3D[xi[0]][xi[1]][xi[2]-1];
                    index = {xi[0],xi[1],xi[2]-1};
                }
                else
                {
                    min_t1 = T3D[xi[0]][xi[1]][xi[2]-1];
                    min_t2 = T3D[xi[0]][xi[1]][xi[2]+1] ;

                    if (min_t1 > min_t2){
                        min_t = min_t2;
                        index = {xi[0],xi[1],xi[2]+1};
                    }
                    else
                    {
                        min_t = min_t1;
                        index = {xi[0],xi[1],xi[2]-1};
                    }                
                }
            }

            else if (axis== 1)
            {
                {
                    if (xi[1] == 0)
                        {
                            min_t = T3D[xi[0]][xi[1]+1][xi[2]];
                            index = {xi[0],xi[1]+1,xi[2]};
                        }
                    else if (xi[1] == n-1)
                        {
        
                            min_t = T3D[xi[0]][xi[1]-1][xi[2]];
                            index = {xi[0],xi[1]-1,xi[2]};
                        }
                        else {
                            min_t1 = T3D[xi[0]][xi[1]-1][xi[2]];
                            min_t2 = T3D[xi[0]][xi[1]+1][xi[2]] ;
        
        
                            if (min_t1 > min_t2){
                                min_t = min_t2;
                                index = {xi[0],xi[1]+1,xi[2]};
                            }
                            else{
                                index = {xi[0],xi[1]-1,xi[2]};
                                min_t = min_t1;
                            }
                    }
                    }
            }
            else 
                {
                    if (xi[0] == 0)
                    {
                        min_t = T3D[xi[0]+1][xi[1]][xi[2]];
                        index = {xi[0]+1,xi[1],xi[2]};
                    }
                    else if (xi[0] == n-1)
                    {
                        index = {xi[0]-1,xi[1],xi[2]};
                        min_t = T3D[xi[0]-1][xi[1]][xi[2]];
                    }
                    else {
                        min_t1 = T3D[xi[0]-1][xi[1]][xi[2]];
                        min_t2 = T3D[xi[0]+1][xi[1]][xi[2]] ;
    
    
                        if (min_t1 > min_t2)
                    {       min_t = min_t2;
                            index = {xi[0]+1,xi[1],xi[2]};
                    }
                        else{
                            index = {xi[0]-1,xi[1],xi[2]};
                            min_t = min_t1;
                    }}
                }
            if ((min_t < 1e3) && (min_t < T3D[xi[0]][xi[1]][xi[2]])){
                liste.push_back(min_t);
                
            }
            else
                a--;
            
            indice.push_back(index);
        }
    return a ;
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

double SolveNdim3D(const int *xi,int dim,const std::vector<double> &liste,const std::vector<std::vector<std::vector<double>>> &F,double h)
{   double f;
    f = F[xi[0]][xi[1]][xi[2]];
    if (liste[0] == 0){
        return liste[0]+ h/f;
    }
    if (dim == 0) {
        return liste[0]+ h/f;
    }
    
    double sumT = 0;
    double sumT2 = 0 ;
    
    for (double i = 0  ; i <= dim ; i++  )
    {
        sumT += liste[i];
        sumT2 += liste[i]*liste[i];
    }

    double a = dim+1; // 1,2 ou 3
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
    }
}


/**
 * @brief résous l'équation Eikonal 2d et 3d 
 * 
 * xi : point on grid, is a liste of len(2)
 * n  : number of point in x and y
 * h  : steps delta_x
 * F  : double List of speed associated to each point
 * T  : double List of distance associated to each point 
 */

double SolveEikonale3D(const int *xi, const std::vector<std::vector<std::vector<double>>> &T,const std::vector<std::vector<std::vector<double>>> &F,int n,double h,std::array<int,3> &closest)
{
    double inf = std::numeric_limits<double>::infinity();
    std::vector<double> liste;
    int a ;
    
    //std::cout << " xi egale : " << xi[0] << " " << xi[1] << " " <<  xi[2] << std::endl;
    std::array<int, 3> ind_1 ;
    std::array<int, 3> ind_2 ;
    std::array<int, 3> ind_3 ;
    std::vector<std::array<int,3>> indice;
    // std::cout << "on cherche le min " << std::endl ; 
    a = findMinValues3D(T,xi,n,liste,indice) ; // doit donnée 0 ,1 ,2 ou 3 
    if (a == 0) {
        return inf;
    }   
    
    std::sort(liste.begin(), liste.end());
    double ti_tild = 0;
    //std::cout << "liste T voisin : " ;
    //for(auto val : liste  ){
    //   std::cout << val << " ";
    //}
    //std::cout << std::endl;
    // on boucle sur combiens de dimension ont donné une distance finies,
    for(int dim = 0 ; dim < a; dim++) {
        ti_tild = SolveNdim3D(xi,dim,liste,F,h);

        if  (ti_tild < liste[dim+1])
            break ;
    }
    //std::cout << "ti trouve : " << ti_tild << std::endl;

    ind_1 = indice[0];
    ind_2 = indice[1];
    ind_3 = indice[2];
    double dist1 = T[ind_1[0]][ind_1[1]][ind_1[2]];
    double dist2 = T[ind_2[0]][ind_2[1]][ind_2[2]];
    double dist3 = T[ind_3[0]][ind_3[1]][ind_3[2]];

    if (dist1 > dist2 ){
        if(dist2 > dist3)
            closest = ind_3;
        else
            closest = ind_2;
    }
    else{
        if(dist1 > dist3)
            closest = ind_3;
        else
            closest = ind_1;
    }

    
    return ti_tild;
      

}
