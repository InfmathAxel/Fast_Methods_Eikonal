#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <cmath>
#include <sstream>  // Pour construire la chaîne formatée
#include <fstream>
#include <chrono>
#include "SolveEikonal3D.hpp"

#pragma once


void getSweepDirection(int N,int * &direction) {
    /**
     * @brief récupère les directions dans laquels on va sweep selon la précédentes
     *
     * N :the dimension of our mesh
     * direction : list of N int, composed of 1 and -1 to show the direction we esplore on the specified index
    */
    for(int i = 0 ; i < N ; i ++) {
        direction[i] += 2 ;
        if (direction[i] <= 1)
            break;
        else
            direction[i] = -1;
    }
}


bool Sweep(std::vector<std::vector<std::vector<double>>> &T,std::vector<std::vector<std::vector<double>>> &Closest,std::vector<std::vector<std::vector<double>>> &Lock, const std::vector<std::vector<std::vector<double>>> &F,const int* sweep_dir,int N,int n, int m,double h) {
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
    int width = n;
    int a = 0 ;
    double eps = 1e-2;

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
                        if (sweep_dir[2] ==0)
                            for(int k = 0 ; k < width ; k++)
                            {
                                std::array<int,3> close;

                                int x[3] = {i,j,k};
                                double Ti = SolveEikonale3D(x,T,F,n,h,close);
                                if (Ti <= T[i][j][k])
                                { if (std::abs(Ti - T[i][j][k] ) > eps )
                                    stop = false;   
                                    T[i][j][k] = Ti;

                                }
                            }
                        else {
                            for(int k = width-1 ; k >= 0 ; k--)
                            {
                                std::array<int,3> close;

                                int x[3] = {i,j,k};
                                double Ti = SolveEikonale3D(x,T,F,n,h,close);
                                if (Ti <= T[i][j][k])
                                { if (std::abs(Ti - T[i][j][k] ) > eps )
                                    stop = false;
                                    T[i][j][k] = Ti;

                                }
                            }
                        }
                    }
                }
                else
                {
                    for (int j = cols - 1; j >= 0; j--)
                    {
                        if (sweep_dir[2] ==0)
                            for(int k = 0 ; k < width ; k++)
                            {
                                std::array<int,3> close;

                                int x[3] = {i,j,k};
                                double Ti = SolveEikonale3D(x,T,F,n,h,close);
                                if (Ti <= T[i][j][k])
                                { if (std::abs(Ti - T[i][j][k] ) > eps )
                                    stop = false;
                                    T[i][j][k] = Ti;

                                }
                            }

                        else {
                            for(int k = width-1 ; k >= 0 ; k--)
                            {
                                std::array<int,3> close;

                                int x[3] = {i,j,k};
                                double Ti = SolveEikonale3D(x,T,F,n,h,close);
                                if (Ti <= T[i][j][k])
                                { if (std::abs(Ti - T[i][j][k] ) > eps )
                                    stop = false;
                                    T[i][j][k] = Ti;

                                }
                            }
                        }


                    }
                }
            }
        } else 
        {
            // Balayage croissant de X puis croissant ou decroissant de Y
            for (int i = rows - 1; i >= 0; i--)
            {
                if (sweep_dir[1] == 1)
                {
                    for (int j = 0; j < cols; j++)
                    {
                        if (sweep_dir[2] ==0)
                            for(int k = 0 ; k < width ; k++)
                            {
                                std::array<int,3> close;

                                int x[3] = {i,j,k};
                                double Ti = SolveEikonale3D(x,T,F,n,h,close);
                                if (Ti <= T[i][j][k])
                                { if (std::abs(Ti - T[i][j][k] ) > eps )
                                    stop = false;
                                    T[i][j][k] = Ti;

                                }
                            }
                        else {
                            for(int k = width-1 ; k >= 0 ; k--)
                            {
                                std::array<int,3> close;

                                int x[3] = {i,j,k};
                                double Ti = SolveEikonale3D(x,T,F,n,h,close);
                                if (Ti <= T[i][j][k])
                                { if (std::abs(Ti - T[i][j][k] ) > eps )
                                    stop = false;
                                    T[i][j][k] = Ti;

                                }
                            }
                        }
                    }
                }
                else
                {
                    for (int j = cols - 1; j >= 0; j--)
                    {
                        if (sweep_dir[2] ==0)
                            for(int k = 0 ; k < width ; k++)
                            {
                                std::array<int,3> close;

                                int x[3] = {i,j,k};
                                double Ti = SolveEikonale3D(x,T,F,n,h,close);
                                if (Ti <= T[i][j][k])
                                { if (std::abs(Ti - T[i][j][k] ) > eps )
                                    stop = false;
                                    T[i][j][k] = Ti;

                                }
                            }

                        else {
                            for(int k = width-1 ; k >= 0 ; k--)
                            {
                                std::array<int,3> close;

                                int x[3] = {i,j,k};
                                double Ti = SolveEikonale3D(x,T,F,n,h,close);
                                if (Ti <= T[i][j][k])
                                { if (std::abs(Ti - T[i][j][k] ) > eps )
                                    stop = false;
                                    T[i][j][k] = Ti;

                                }
                            }
                        }


                    }
                }
            }
        }

    }
    return stop;
}