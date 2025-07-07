#include "SolveEikonal2D.hpp"
#include <unordered_set>
#include "sweep2D.hpp"

// 
#pragma once 


double FSM2D(std::vector<std::vector<double>> &T,int n,double lenght,const std::vector<std::pair<int,int>> &Xs,std::vector<std::vector<double>> &Closest )
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
    {   int i = std::get<0>(xi);
        int j = std::get<1>(xi);
        T[i][j] = 0 ;
        Closest[i][j] = compteur_Xs;
        compteur_Xs++ ;
    }

    int N = 2 ;

    int* sweep_direction = new int[2] {1, 1};
    bool stop = false;
    int compteur = 0 ; 
    // du tant que l'on n'a pas sweep dans toutes les directions on itère: 
    while (!stop) {
        getSweepDirection(N,sweep_direction);
        //std::cout << "direction " << sweep_direction[0] << " " << sweep_direction[1] << std::endl;
        compteur ++;
        stop = Sweep(T,Closest,F,sweep_direction,N,n,m,h);
        
    }
    std::cout << "nombre de sweep : " << compteur << std::endl;
    delete[] sweep_direction;

    auto end_t = std::chrono::high_resolution_clock::now();

    // Calculer la durée écoulée
    std::chrono::duration<double> duration = end_t - start_t;

    // Afficher le temps écoulé
    std::cout << "Temps d'exécution FSM : " << duration.count() << " secondes." << std::endl;

    return duration.count();


}