#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <cmath>
#include <sstream>  // Pour construire la chaîne formatée
#include <fstream>
#include <chrono>
#include "solveEikonal.hpp"
#pragma once 
void print_T(std::vector<std::vector<double>> &T){
    for (const auto &row : T) {
        for (double value : row) {
            std::cout << value << "\t";  // Affichage avec tabulation
        }
        std::cout << std::endl;
    }
}

void print(std::vector<std::vector<double>> &Closest){
    
    for (auto &raw : Closest){
        for(auto &col : raw){
            std::cout << col << " " ; 

        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

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

struct pair_hash {
    std::size_t operator()(const std::pair<int, int>& p) const {
        return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
    }
};



bool Sweep(std::vector<std::vector<double>> &T,std::vector<std::vector<double>> &Closest, const std::vector<std::vector<double>> &F,const int* sweep_dir,int N,int n, int m,double h, std::unordered_set<std::pair<int,int>, pair_hash> frozen) {
    /**
     * @brief  Parcours le maillage par sweep dans la direction sweep_dir
     * 
     * - T matrice des distances
     * - Closest matrice des tags de la source la plus proches
     * - F matrice des vitesses associés à chaque point 
     * - sweep_dir : direction des sweeps selon chaque axe 
     * - N la dimension du problème 
     * - n,m  nombre de point selon x et y 
     * - h le pas selon x et y
    */
    bool stop = true;
    int rows = n;
    int cols = m ;
    int a = 0 ;
    
    if (N-1 > 1)
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

                        Sweep(T,Closest, F, sweep_dir, N - 1,n,m,h,frozen);
                    }
                } else 
                {
                    for (int j = cols - 1; j >= 0; j--) 
                    {

                        Sweep(T, Closest,F, sweep_dir, N - 1,n,m,h,frozen);
                    }
                }
            }
        }
        else
        {
            // Balayage croissant de X puis croissant ou decroissant de Y
            for (int i = rows - 1; i >= 0; i--)
            {
                if (sweep_dir[1] == 1) {
                    for (int j = 0; j < cols; j++)
                    {
                        Sweep(T,Closest, F, sweep_dir, N - 1,n,m,h,frozen);
                    }
                } else
                {

                    for (int j = cols - 1; j >= 0; j--)
                    {
                        Sweep(T, Closest,F, sweep_dir, N - 1,n,m,h,frozen);
                    }
                }
            }
        }
    }


    else
    {   //std::cout << "oui" << std::endl ; 
        if (sweep_dir[0] == 1)
        {
            // Balayage croissant de X puis croissant ou decroissant de Y
            for (int i = 0; i < rows; i++)
            {
                if (sweep_dir[1] == 1)
                {
                    for (int j = 0; j < cols; j++)
                    {   
                        
                        std::pair<int,int> close ;
                        int x[2] = {i,j};
                        //if (frozen.find({i,j}) == frozen.end())
                        {
                        double Ti = SolveEikonale_closest(x,T,F,n,h,close);
                        //std::cout << "working on : " << x[0]  << "," << x[1] << " closest " << close.first <<","<<  close.second << std::endl;
                        if (Ti < T[i][j])
                        {
                            T[i][j] = Ti;
                            
                            Closest[i][j] = Closest[close.first][close.second] ;
                            
                            stop = false;
                        }
                      //std::cout << "closest : " << std::endl;
                      //print(Closest);
                      //std::cout << "T  :" << std::endl;
                      //print(T);
                    
                      //std::cin >> a ;
                    }
                }
                } else
                {
                    for (int j = cols - 1; j >= 0; j--)
                    {   std::pair<int,int> close ;
                        int x[2] = {i,j};
                        //if (frozen.find({i,j}) == frozen.end())
                        {
                        double Ti = SolveEikonale_closest(x,T,F,n,h,close);
                        //std::cout << "working on : " << x[0]  << "," << x[1] << " closest " << close.first <<","<<  close.second << std::endl;
                        if (Ti < T[i][j])
                        {
                            T[i][j] = Ti;
                           
                            Closest[i][j] = Closest[close.first][close.second] ;
                                
                            stop = false;
                        }
                        //std::cout << "closest : " << std::endl;
                        //print(Closest);
                        //std::cout << "T  :" << std::endl;
                        //print(T);
                        //std::cin >> a ;

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
                        {   std::pair<int,int> close ;
                            int x[2] = {i,j};
                            //if (frozen.find({i,j}) == frozen.end())
                            {
                            double Ti = SolveEikonale_closest(x,T,F,n,h,close);
                            //std::cout << "working on : " << x[0]  << "," << x[1] << " closest " << close.first <<","<<  close.second << std::endl;
                            if (Ti < T[i][j])
                            {
                                T[i][j] = Ti;
                                
                                Closest[i][j] = Closest[close.first][close.second] ;
                                    
                                stop = false;
                            }
                           //std::cout << "closest : " << std::endl;
                        //print(Closest);
                        //std//::cout << "T  :" << std::endl;
                        //pri//nt(T);
                        //std//::cin >> a ;
                        }}
                    
                } else
                {
                    for (int j = cols - 1; j >= 0; j--)
                    {
                        {   std::pair<int,int> close ;
                            int x[2] = {i,j};
                            //if (frozen.find({i,j}) == frozen.end())
                            {
                            double Ti = SolveEikonale_closest(x,T,F,n,h,close);
                            //std::cout << "working on : " << x[0]  << "," << x[1] << " closest " << close.first <<","<<  close.second << std::endl;
                            if (Ti < T[i][j])
                            {
                                T[i][j] = Ti;
                                
                                Closest[i][j] = Closest[close.first][close.second] ;
                                    
                                stop = false;
                            }
                        //std::cout << "closest : " << std::endl;
                        //print(Closest);
                        //std::cout << "T  :" << std::endl;
                        //print(T);
                        //std::cin >> a ;
                        }}
                    }
                }
            }
        }
    }
    
return stop;
}



