#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <cmath>
#include <sstream>  // Pour construire la chaîne formatée
#include <fstream>
#include <chrono>
#pragma once 

double SolveNdim(const int *xi,int dim,const std::vector<double> &liste,const std::vector<std::vector<double>> &F,double h)
{   //std::cout << "dim = " << dim << std::endl;
    if (dim == 0) {

        //std::cout << h << "" << F[xi[0]][xi[1]] ;
        return liste[0]+ h/F[xi[0]][xi[1]];
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
    double c = sumT2 - h*h / F[xi[1]][xi[0]];

    double q = b*b - 4*a*c;

    if (q < 0)
        return std::numeric_limits<double>::infinity();
    else
        return (-b + std::sqrt(q) )/ (2 * a) ;

}





double SolveEikonale(const int *xi, const std::vector<std::vector<double>> &T,const std::vector<std::vector<double>> &F,int n,double h)
{
    /**
    * xi : point on grid, is a liste of len(2)
    * n  : number of point in x and y
    * h  : steps delta_x
    * F  : double List of speed associated to each point
    * T  : double List of distance associated to each point
    */
    //auto start_t = std::chrono::high_resolution_clock::now();
    double inf = std::numeric_limits<double>::infinity();

    double min_t1,min_t2;
    int a = 2;
    std::vector<double> liste;
    double min_t = 0;

    for (int dim = 0; dim <2 ; dim++)
    {
        if (dim == 0)
        {
            if (xi[1] == 0)
                min_t = T[xi[0]][xi[1]+1];
            else if (xi[1] == n-1)
                min_t = T[xi[0]][xi[1]-1];
            else
            {
                min_t1 = T[xi[0]][xi[1]-1];
                min_t2 = T[xi[0]][xi[1]+1] ;

                if (min_t1 > min_t2)
                    min_t = min_t2;
                else
                    min_t = min_t1;
            }
        }

        else
        {   //std::cout << "dim = "<< dim << std::endl;
            if (xi[0] == 0)
            {   //std::cout << "entree ici : min_t = " << min_t << std::endl ;
                min_t = T[xi[0]+1][xi[1]];
                //std::cout << "min_t après changement : "<< min_t << std::endl ;
            }
            else if (xi[0] == n-1)
            {   //std::cout << "entree ici AUSSI: min_t = " << min_t << std::endl ;

                min_t = T[xi[0]-1][xi[1]];
            }
            else {
                min_t1 = T[xi[0]-1][xi[1]];
                min_t2 = T[xi[0]+1][xi[1]] ;
            

            if (min_t1 > min_t2)
                min_t = min_t2;
            else
                min_t = min_t1;
            }
        }
        //std::cout <<"xi min_t T "<< xi[0] << "  " << xi[1] <<  " " << min_t <<" " << T[xi[0]][xi[1]] << std::endl ;
        //std::cout << "min_t = "<<  min_t << std::endl;
        if ((min_t < 1e3) && (min_t < T[xi[0]][xi[1]]))

            liste.push_back(min_t);
        else
            a--;
    }

    if (a == 0) {
        //std::cout << "a == 0 \n";
        return inf;
    }

    //auto end_t = std::chrono::high_resolution_clock::now();
    //std::chrono::duration<double> duration = end_t - start_t;
   
    // Afficher le temps écoulé
    //std::cout << "Temps d'exécution search min   : " << duration.count() << " secondes." << std::endl;
    //auto start_j = std::chrono::high_resolution_clock::now();
    std::sort(liste.begin(), liste.end());
    //for(auto val : liste)
        //std::cout << "val =  " << val << std::endl  ;
    double ti_tild = 0;

    for(int dim = 0 ; dim < a; dim++) {
        ti_tild = SolveNdim(xi,dim,liste,F,h);

        if ((dim == a-1) or (ti_tild < liste[dim+1]))
            break ;
    }
    //auto end_j = std::chrono::high_resolution_clock::now();
    //std::chrono::duration<double> duration2 = end_j - start_j;
   
    // Afficher le temps écoulé
    //std::cout << "Temps d'exécution solvendim  : " << duration2.count() << " secondes." << std::endl;
    return ti_tild;


}