#include "solveEikonal.hpp"
#include "database.hpp"
#include <unordered_set>
#include "sweep.hpp"
#include "debug.hpp"
#include "maille.hpp"
#include "source.hpp"
// 
#pragma once 



double FSM(std::vector<std::vector<double>> &T,int n,double lenght,const std::vector<source> &Xs,std::vector<std::vector<double>> &Closest,meshes mesh )

/**
 * @brief fonction pour appliquer la Fast Sweeping Methode(FSM)
 *
 * Cette méthode ce base sur des balayements du maillage dans les directions cardinales selon le nombre de dimension
 * On va parcourir les maillages dans chaques directions et résoudre le problème eikonal à chaque point ce qui fais 
 * que l'on va itèrer sur n points fois le nombre de balayage. 
 * 
 * -T :Matrice qui stocke les distances
 * -n : nombre de point selon x et y
 * -lenght : longueur de l'axe x et y
 * -Xs : vecteur contenant l'indice des sources
 * -Closest : Matrice contenant le tag de la source la plus proche
 
 */
{
    auto start_t = std::chrono::high_resolution_clock::now();
    //initialisation
    int m = n ;
    double h = lenght/(n-1);
    std::vector<std::vector<double>> F(n, std::vector<double>(m, 1.0));

    double compteur_Xs = 1;
    for (auto xi : Xs)
    {   std::pair<int,int> ind = xi.get_ind();
        std::unordered_set<std::pair<int,int>, pair_hash> frozen;
        int i = ind.first;
        int j = ind.second;
        T[i][j] = 0 ;
        Closest[i][j] = compteur_Xs;
        compteur_Xs++ ;
    }

    std::unordered_set<std::pair<int,int>, pair_hash> frozen;
    
    for (auto xi : Xs)
    {   std::pair<int,int> ind = xi.get_ind();
        int i = ind.first;
        int j = ind.second;
        
        
        double x = xi.get_x();
        double y = xi.get_y();
        coords source(x,y);

        std::vector<std::pair<int,int>> direction = {{-1,0},{1,0},{0,-1},{0,1}};
        for (auto& direc : direction)
        {
            int di = direc.first + i;
            int dj = direc.second + j;
            

            if ((0 <= di ) && (di < n ) && (0 <= dj) && (dj < m )){
                coords voisin = mesh.get_coord(di,dj);
                double ti = eucli(voisin,source); 
                if (T[di][dj] > ti){
                    T[di][dj] = ti;
                    Closest[di][dj] = Closest[i][j];
                }
                frozen.insert({di,dj});
        }}
        frozen.insert({i,j});
    }

    int N = 2 ;

    int* sweep_direction = new int[2] {1, 1};
    bool stop = false;

    // du tant que l'on n'a pas sweep dans toutes les directions on itère: 
    while (!stop) {
        getSweepDirection(N,sweep_direction);
        stop = Sweep(T,Closest,F,sweep_direction,N,n,m,h,frozen);

    }

    delete[] sweep_direction;

    auto end_t = std::chrono::high_resolution_clock::now();

    // Calculer la durée écoulée
    std::chrono::duration<double> duration = end_t - start_t;

    // Afficher le temps écoulé
    std::cout << "Temps d'exécution FSM : " << duration.count() << " secondes." << std::endl;

    return duration.count();


}