#include "solveEikonal.hpp"
#include "database.hpp"
#include <unordered_set>
#include "debug.hpp"
#include "maille.hpp"
#include "source.hpp"
#include <omp.h>
// 
#pragma once 




double FIM(std::vector<std::vector<double>> &T,int n,double lenght,const std::vector<source> &Xs,std::vector<std::vector<double>> &Closest,meshes mesh )

/** 
 * @brief fonction pour appliquer la Fast Iterative Methode(FIM)
 *
 *  Cet méthode ce base sur le parcours d'une bande active ou l'on stocke les voisins 
 *  des points sur lesquels nous avons déjà travaillé et des interfaces  pour être sur de pouvoir avancer vers la convergence 
 *  du calcule de distance en ces points et ne pas repassé de trop nombreuse fois sur eux en faisant des calculs inutiles  
 *
 * -T :Matrice qui stocke les distances
 * -n : nombre de point selon x et y
 * -lenght : longueur de l'axe x et y
 * -Xs : vecteur contenant l'indice des sources
 * -Closest : Matrice contenant le tag de la source la plus proche
 */

{
    auto start_t = std::chrono::high_resolution_clock::now();
    int m = n ;
    double h = lenght/(n-1);
     
    std::vector<std::vector<double>> F(n, std::vector<double>(m, 1.0));
    double compteur_xs = 1 ;
    for (auto xi : Xs)
    {   std::pair<int,int> ind = xi.get_ind();
        int i = ind.first;
        int j = ind.second;
        T[i][j] = 0 ; 
        Closest[i][j] = compteur_xs;
        compteur_xs++;
    }
    

    // unordered set to have easy acess to the pair of index and easy acess to find method
    
    
    
    
    double eps = 1e-2;
    std::pair<int,int> close ;
    // boucle on narrow set
    #pragma omp parallel
    {
        #pragma omp single
        {
            for (int idx = 0; idx < Xs.size(); ++idx) 
            {
                #pragma omp task firstprivate(idx)
                {   
                    std::unordered_set<std::pair<int,int>, pair_hash> narrow;
                    source xi = Xs[idx]; 
                    //std::cout << compteur << std::endl;
                    std::pair<int,int> ind = xi.get_ind();
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
                                narrow.insert({di,dj});
                        }
                        }
                    

                    int iter = 0;
                    while (!narrow.empty() && iter < 100000) {
                        std::vector<std::pair<int, int>> copie_narrow_vec(narrow.begin(), narrow.end());
                        std::unordered_set<std::pair<int, int>, pair_hash> copie_narrow(narrow);
                        iter ++;
                        
                        
                        for (const auto &pair : copie_narrow_vec) {
                            int i = std::get<0>(pair);
                            int j = std::get<1>(pair);
                            int xi[2] = {i, j};
                            double ti = T[i][j];
                            double T_tild = SolveEikonale(xi, T, F, n, h);
                            T[i][j] = T_tild;

                            std::vector<std::pair<int, int>> neighboor;
                            for (auto &direc : direction) {
                                int di = direc.first + std::get<0>(pair);
                                int dj = direc.second + std::get<1>(pair);

                                if ((0 <= di) && (di < n) && (0 <= dj) && (dj < m)) {
                                    neighboor.push_back({di, dj});
                                }
                            }

                            if (std::abs(ti - T_tild) < eps) {
                                for (const auto &pair2 : neighboor) {
                                    if ((copie_narrow.find(pair2) == copie_narrow.end())) {
                                        int i2 = std::get<0>(pair2);
                                        int j2 = std::get<1>(pair2);
                                        int xj[2] = {i2, j2};
                                        double ti2 = T[i2][j2];
                                        double T_tild2 = SolveEikonale(xj, T, F, n, h);
                                        if (ti2 > T_tild2) {
                                            T[i2][j2] = T_tild2;
                                            narrow.insert({i2, j2});
                                        }
                                    }
                                }
                                narrow.erase({i, j});
                            }
                        }
                        
                        
                    }

                    
                   

                }
            }
        }
    }


    auto end_t = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_t - start_t;

    // Afficher le temps écoulé
    std::cout << "Temps d'exécution FIM : " << duration.count() << " secondes." << std::endl;
    return duration.count();
}