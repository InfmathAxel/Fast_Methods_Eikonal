#include "solveEikonal.hpp"
#include "database.hpp"
#include <unordered_set>
#include "debug.hpp"
#include "maille.hpp"
#include "source.hpp"
// 
#pragma once 

// hash function to stock std::pair in unordered set
struct pair_hash_fim 
/**
 * stucture de hashage pour stocker des std::pair<int,int> dans un unordered set 
 */
{
    std::size_t operator()(const std::pair<int, int>& p) const {
        return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
    }
};




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
    std::unordered_set<std::pair<int,int>, pair_hash_fim> narrow;
    
    // stock all neighbord of source in
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
                narrow.insert({di,dj});
        }}
    }
    
    double eps = 1e-2;
    std::pair<int,int> close ;
    // boucle on narrow set
    while (!narrow.empty())
    {
        std::unordered_set<std::pair<int,int>, pair_hash_fim>  copie_narrow(narrow);
        for (const auto& pair : copie_narrow)
        {   
            // on compare la distance stockée dans T et la distance calculé avec SolveEikonale 
            int xi[2] = {std::get<0>(pair),std::get<1>(pair)};
            double ti = T[xi[0]][xi[1]];
            double Ti_tild = SolveEikonale_closest(xi, T, F, n, h,close);
            if (Ti_tild < ti)
            T[xi[0]][xi[1]] = Ti_tild;
            Closest[xi[0]][xi[1]] = Closest[std::get<0>(close)][std::get<1>(close)];
            // Si le seuil est dépassé on calcule une première fois T pour les voisins de xi et on les stock dans narrows
            if (std::abs(Ti_tild-ti) < eps)
            {   // on cherche les voisins des xi 
                std::vector<std::pair<int,int>> x_neighboor;
                std::vector<std::pair<int,int>> direction = {{-1,0},{1,0},{0,-1},{0,1}};
                for (auto& direc : direction)
                {
                    int di = direc.first + xi[0];
                    int dj = direc.second + xi[1];

                    if ((0 <= di ) && (di < n ) && (0 <= dj) && (dj < m ))
                        x_neighboor.push_back({di,dj});
                }

                for (auto& x_nb : x_neighboor)
                {   // pour tout les voisins on calcule T et on ajoute xi à narrow si le points n'y est pas déjà 
                    int x = std::get<0>(x_nb);
                    int y = std::get<1>(x_nb);

                    if ((copie_narrow.find({x,y})== copie_narrow.end()))
                    {   int xj[2] = {x,y};
                        double p = T[x][y];
                        double q = SolveEikonale(xj, T, F, n, h);
                        if (p > q)
                        {
                            T[xj[0]][xj[1]] = q;
                            
                            Closest[xj[0]][xj[1]] = Closest[xi[0]][xi[1]];
                           
                            narrow.insert({xj[0],xj[1]});
                            
                        }
                    }
                }
                narrow.erase(pair);
               
            }


            
        }
        
    }

    auto end_t = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_t - start_t;

    // Afficher le temps écoulé
    std::cout << "Temps d'exécution FIM : " << duration.count() << " secondes." << std::endl;
    return duration.count();
}