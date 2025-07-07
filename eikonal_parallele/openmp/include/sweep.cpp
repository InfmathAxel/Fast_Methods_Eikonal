#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <cmath>
#include <sstream>  // Pour construire la chaîne formatée
#include <fstream>
#include <chrono>
#include "solveEikonal.cpp"
#include <omp.h>
#pragma once 
void getSweepDirection(int N,int * &direction) {
    /*
    @param N :the dimension of our mesh
    @param direction : list of N int, composed of 1 and -1 to show the direction we esplore on the specified index 
    */
    for(int i = 0 ; i < N ; i ++) {
        direction[i] += 2 ;
        if (direction[i] <= 1)
            break;
        else
            direction[i] = -1;
    }
}

bool Sweep(std::vector<std::vector<double>> &T, const std::vector<std::vector<double>> &F,const int* sweep_dir,int N,int n, int m,double h) {
    /*

    */
    bool stop = true;
    int rows = n;
    int cols = m ;
    double  compteur = 0 ;
    double eps = 1e-5;
    double diff = 0 ; 
  

    {
        if (sweep_dir[0] == 1)
        {
            // Balayage croissant de X puis croissant ou decroissant de Y
            for (int i = 0; i < rows; i++)
            {
                if (sweep_dir[1] == 1)
                {
                    for (int j = 0; j < cols; j++)
                    {
                        int x[2] = {i,j};
                        double Ti = SolveEikonale(x,T,F,n,h);
                        
                        
                        if (Ti < T[i][j])
                        {   diff += std::abs(Ti - T[i][j]); 
                            T[i][j] = Ti;
                            stop = false;
                            compteur ++;
                            
                            
                        }

                    }
                } else
                {
                    for (int j = cols - 1; j >= 0; j--)
                    {
                        int x[2] = {i,j};
                        double Ti = SolveEikonale(x,T,F,n,h);
                        
                        if (Ti < T[i][j])
                        {   diff += std::abs(Ti - T[i][j]); 
                            T[i][j] = Ti;
                            stop = false;
                            compteur ++;
                            
                        }

                    }
                }
            }
        } else {
            // Balayage croissant de X puis croissant ou decroissant de Y
            for (int i = rows - 1; i >= 0; i--)
            {
                if (sweep_dir[1] == 1)
                {
                    for (int j = 0; j < cols; j++)
                    {
                        {
                            int x[2] = {i,j};
                            double Ti = SolveEikonale(x,T,F,n,h);
                            if (Ti < T[i][j])
                            {   diff += std::abs(Ti - T[i][j]); 
                                T[i][j] = Ti;
                                stop = false;
                                compteur ++;
                                
                            }
                        }
                    }
                } else
                {
                    for (int j = cols - 1; j >= 0; j--)
                    {
                        {
                            int x[2] = {i,j};
                            double Ti = SolveEikonale(x,T,F,n,h);
                            if (Ti < T[i][j])
                            {   diff += std::abs(Ti - T[i][j]); 
                                T[i][j] = Ti;
                                stop = false;
                                compteur ++;
                                
                            }
                        }
                    }
                }
            }
        }
    }
    if (diff/compteur < eps ){
        stop = true;
    }
    #pragma omp critical 
    std::cout << "nb valeur changer : " << compteur << " moyenne de changement : " << diff/compteur<<  std::endl;
    return stop;
}




bool Sweep_seuil(std::vector<std::vector<double>> &T, const std::vector<std::vector<double>> &F,const int* sweep_dir,double seuil,int N,int n, int m,double h) {
    /*

    */
    bool stop = true;
    int rows = n;
    int cols = m ;
    double  compteur = 0 ;
    double eps = 1e-5;
    double diff = 0 ; 
    bool calcul = true;

    {
        if (sweep_dir[0] == 1)
        {   
            // Balayage croissant de X puis croissant ou decroissant de Y
            for (int i = 0; i < rows; i++)
            {   
                if (sweep_dir[1] == 1)
                {
                    for (int j = 0; j < cols; j++)
                    {
                        int x[2] = {i,j};
                        double Ti = SolveEikonale(x,T,F,n,h);
                        if (Ti > seuil)
                            calcul = false;
                        if(Ti <= seuil )
                            calcul = true;
                        if ((Ti < T[i][j]) && calcul)
                        {   diff += std::abs(Ti - T[i][j]); 
                            T[i][j] = Ti;
                            stop = false;
                            compteur ++;
                            
                            
                        }

                    }
                } else
                {
                    for (int j = cols - 1; j >= 0; j--)
                    {
                        int x[2] = {i,j};
                        double Ti = SolveEikonale(x,T,F,n,h);
                        if (Ti > seuil)
                            calcul = false;
                        if(Ti <= seuil )
                            calcul = true;
                       if ((Ti < T[i][j]) && calcul)
                        {   diff += std::abs(Ti - T[i][j]); 
                            T[i][j] = Ti;
                            stop = false;
                            compteur ++;
                            
                        }

                    }
                }
            }
        } else {
            // Balayage croissant de X puis croissant ou decroissant de Y
            for (int i = rows - 1; i >= 0; i--)
            {   
                if (sweep_dir[1] == 1)
                {   
                    for (int j = 0; j < cols; j++)
                    {
                        {
                            int x[2] = {i,j};
                            double Ti = SolveEikonale(x,T,F,n,h);
                            if (Ti > seuil)
                                calcul = false;
                            if(Ti <= seuil )
                                calcul = true;
                           if ((Ti < T[i][j]) && calcul)
                            {   diff += std::abs(Ti - T[i][j]); 
                                T[i][j] = Ti;
                                stop = false;
                                compteur ++;
                                
                            }
                        }
                    }
                } else
                {
                    for (int j = cols - 1; j >= 0; j--)
                    {
                        {
                            int x[2] = {i,j};
                            double Ti = SolveEikonale(x,T,F,n,h);
                            if (Ti > seuil)
                                calcul = false;
                            if(Ti <= seuil )
                                calcul = true;
                           if ((Ti < T[i][j]) && calcul)
                            {   diff += std::abs(Ti - T[i][j]); 
                                T[i][j] = Ti;
                                stop = false;
                                compteur ++;
                                
                            }
                        }
                    }
                }
            }
        }
    }
    if (diff/compteur < eps ){
        stop = true;
    }
    #pragma omp critical 
    std::cout << "nb valeur changer : " << compteur << " moyenne de changement : " << diff/compteur<<  std::endl;
    return stop;
}



