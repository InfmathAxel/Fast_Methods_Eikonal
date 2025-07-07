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
/**
 * @brief Résouds le problème quadratique associé à l'équation eikonale 
 * 
 * -xi : point où l'on résouds notre problème 
 * -dim : nombre de point dans liste 
 * -liste : liste des distances des voisins 
 * -F : vitesse associés au points xi 
 * -h : le pas selon x et y  
 */
{   
    if (dim == 0) {
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
     * @brief Résouds le problème eikonale au point xi; 
     *  
     * xi : point on grid, is a liste of len(2)
     * n  : number of point in x and y
     * h  : steps delta_x
     * F  : double List of speed associated to each point
     * T  : double List of distance associated to each point
    */

    double inf = std::numeric_limits<double>::infinity();

    double min_t1,min_t2;
    int a = 2;
    std::vector<double> liste;
    double min_t = 0;

    for (int dim = 0; dim <2 ; dim++)
    // boucle sur la dimension du probleme (ici on a que pour 1d et 2d, le 3d n'est pas implementé)
    // on vérifie les voisin du point par rapport a une direction selon la dimension sur laquel on regarde
    // (haut bas pour 2d selon l'axe y , gauche droite pour 1d selon l'axe xet pour 3d se serait devant derrière ? selon l'axe z ) 
    // on stock aussi l'indice des voisins les plus proches selon les axes dans ind_1 et ind_2
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
        {
            if (xi[0] == 0)
            {
                min_t = T[xi[0]+1][xi[1]];

            }
            else if (xi[0] == n-1)
            {

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

        if ((min_t < 1e3) && (min_t < T[xi[0]][xi[1]]))

            liste.push_back(min_t);
        else
            a--;
    }

    // pas de solutioon 
    if (a == 0) {
        return inf;
    }

    // on trie la liste des distance des voisins 
    std::sort(liste.begin(), liste.end());
    double ti_tild = 0;

    // on boucle sur combiens de dimension ont donné une distance finies,
    for(int dim = 0 ; dim < a; dim++) {
        ti_tild = SolveNdim(xi,dim,liste,F,h);

        if ((dim == a-1) or (ti_tild < liste[dim+1]))
            break ;
    }

    return ti_tild;


}


double SolveEikonale_closest(const int *xi, const std::vector<std::vector<double>> &T,const std::vector<std::vector<double>> &F,int n,double h,std::pair<int,int> &closest)
{
    /**
     * @brief Résouds le problème eikonale au point xi en rajoutant le tag de la source la plus proche ; 
     * 
     * xi : point on grid, is a liste of len(2)
     * n  : number of point in x and y
     * h  : steps delta_x
     * F  : double List of speed associated to each point
     * T  : double List of distance associated to each point
     * closest : pair d'indice qui nous donne l'indice du point qui donne le tag de la source la plus proche pour chaque point
    */


    double inf = std::numeric_limits<double>::infinity();
    double min_t1,min_t2;
    int a = 2;
    std::vector<double> liste;
    double min_t = 0;
    std::pair<int,int> ind_1;
    std::pair<int,int> ind_2;

    for (int dim = 0; dim <2 ; dim++)
    // boucle sur la dimension du probleme (ici on a que pour 1d et 2d, le 3d n'est pas implementé)
    // on vérifie les voisin du point par rapport a une direction selon la dimension sur laquel on regarde
    // (haut bas pour 2d selon l'axe y , gauche droite pour 1d selon l'axe xet pour 3d se serait devant derrière ? selon l'axe z ) 
    // on stock aussi l'indice des voisins les plus proches selon les axes dans ind_1 et ind_2 
    {
        if (dim == 0)
        {
            if (xi[1] == 0) {
                min_t = T[xi[0]][xi[1]+1];
                ind_1 = {xi[0],xi[1]+1};
            }
            else if (xi[1] == n-1) {
                min_t = T[xi[0]][xi[1]-1];
                ind_1 = {xi[0],xi[1]-1};
            }
            else
            {
                min_t1 = T[xi[0]][xi[1]-1];
                min_t2 = T[xi[0]][xi[1]+1] ;

                if (min_t1 > min_t2) {
                    min_t = min_t2;
                    ind_1 = {xi[0],xi[1]+1};
                }
                else {
                    min_t = min_t1;
                    ind_1 = {xi[0],xi[1]-1};
                }
            }
        }

        else
        {
            if (xi[0] == 0)
            {
                min_t = T[xi[0]+1][xi[1]];
                ind_2 = {xi[0]+1,xi[1]};

            }
            else if (xi[0] == n-1)
            {

                min_t = T[xi[0]-1][xi[1]];
                ind_2 = {xi[0]-1,xi[1]};
            }
            else {
                min_t1 = T[xi[0]-1][xi[1]];
                min_t2 = T[xi[0]+1][xi[1]] ;

                if (min_t1 > min_t2)
                {   min_t = min_t2;
                    ind_2 = {xi[0]+1,xi[1]};
                }
                else
                {   min_t = min_t1;
                    ind_2 = {xi[0]-1,xi[1]};
                }
            }
        }

        if ((min_t < 1e3) && (min_t < T[xi[0]][xi[1]]))

            liste.push_back(min_t);
        else
            a--;
    }
    // pas de solutioon 
    if (a == 0) {
        closest = {xi[0],xi[1]};
        return inf;
    }

    // on trie la liste des distance des voisins 
    std::sort(liste.begin(), liste.end());
    double ti_tild = 0;

    // on boucle sur combiens de dimension ont donné une distance finies,
    for(int dim = 0 ; dim < a; dim++) {
        ti_tild = SolveNdim(xi,dim,liste,F,h);

        if ((dim == a-1) or (ti_tild < liste[dim+1]))
            break ;
    }

    
    // on regarde lequels des voisins à données le ti_tilds et on recupère son tag de la source la plus proche pour le propager à xi
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