#include "solveEikonal.hpp"
#include "database.hpp"
#include <unordered_set>
#include "sweep.hpp"
#include "debug.hpp"
#include "maille.hpp"
#include "source.hpp"
//
#pragma once




double FMM(std::vector<std::vector<double>> &T,int n,double lenght,const std::vector<source> &Xs, std::vector<std::vector<double>> &Closest,meshes mesh)

/**
 * @brief fonction pour appliquer la Fast Marching Methode(FMM)
 *
 * Cette méthode ce base sur l'utilisation des classe Narrow, frozen, unknow,
 * tous les points seront mis dans Unknow sauf les sources en Narrow.
 * puis on va boucler du temps que Narrow n'est pas vide, on regardera
 * les voisins du premier élément de Narrow , résoudrons l'équation eikonal dessus une première fois
 * puis les ajouterons dans Narrow si la distance à réduis et enlevera de Unknow,
 * A chaque fin d'itérations on enlève le premier éléments de narrow puis on l'ajoute dans Frozen pour qu'il ne soit plus modifié.
 *
 * -T :Matrice qui stocke les distances
 * -n : nombre de point selon x et y
 * -lenght : longueur de l'axe x et y
 * -Xs : vecteur contenant l'indice des sources
 * -Closest : Matrice contenant le tag de la source la plus proche
 *
*/
{
    auto start_t = std::chrono::high_resolution_clock::now();

    int m = n ;
    double h = lenght/(n-1);
    std::vector<std::vector<double>> F(n, std::vector<double>(m, 1.0));

    Narrow narrow;
    Frozen frozen;
    Unknow unknow;

    for (int i = 0 ; i < n ; i++)
    {
        for (int j = 0; j< m ; j++)
        {
            unknow.add(i,j,T[i][j]);

        }
    }
    int compteur_xs = 1;

    for (source xi : Xs){
        std::pair<int,int> ind = xi.get_ind();
        int i = ind.first;
        int j = ind.second;
        frozen.add(i,j,0);
        T[i][j] = 0;
        
        double x = xi.get_x();
        double y = xi.get_y();
        coords source(x,y);
        Closest[i][j] = compteur_xs;
        compteur_xs++;
        std::vector<std::pair<int,int>> direction = {{-1,0},{1,0},{0,-1},{0,1}};
        for (auto& direc : direction) {
            
            int di = direc.first + i;
            int dj = direc.second + j;
            if ((0 <= di ) && (di < n ) && (0 <= dj) && (dj < m )){
                coords voisin = mesh.get_coord(di,dj);
                double ti = eucli(voisin,source); 
                if (T[di][dj] > ti){
                    T[di][dj] = ti;
                    if (narrow.check(di,dj))
                        narrow.increase(di,dj,ti);
                    if (unknow.check(di,dj))
                    {
                        narrow.push(di,dj,ti);
                        unknow.remove(di,dj);
                    }
                    Closest[di][dj] = compteur_xs;
                }
                }
            
        }
        compteur_xs++;

    }
    //print(Closest);


    double ti = 0 ;
    // Boucle on narrow til narrow is empty
    while (narrow.get_elem() != 0 )
    {
        std::tuple<int,int,double> x_min = narrow.top();
        std::vector<std::pair<int,int>> x_neighboor;
        std::vector<std::pair<int,int>> direction = {{-1,0},{1,0},{0,-1},{0,1}};
        //get neighboor of narrow top element

        for (auto& direc : direction) {
            int di = direc.first + std::get<0>(x_min);
            int dj = direc.second + std::get<1>(x_min);

            if ((0 <= di ) && (di < n ) && (0 <= dj) && (dj < m ))
                x_neighboor.push_back({di,dj});
        }


        // for all neighboor, if neighboor not in frozen do SolveEikonal and add it to narrow or update it if he is already in it
        for (auto& x_nb : x_neighboor) {
            int x = std::get<0>(x_nb);
            int y = std::get<1>(x_nb);

            if (!frozen.check(x,y))
            {   int xi[2] = {x,y};
                ti = SolveEikonale(xi,T,F,n,h);
                if (ti < T[x][y])
                {   T[x][y] = ti;
                    Closest[x][y] = Closest[std::get<0>(x_min)][std::get<1>(x_min)];
                    if (narrow.check(x,y))
                        narrow.increase(x,y,ti);
                    if (unknow.check(x,y))
                    {
                        narrow.push(x,y,ti);
                        unknow.remove(x,y);
                    }
                }

            }

        }
        // on ajoute le narrow.top dans frozen puis on l'enleve de narrow pour ensuite trier la liste des narrows.
        frozen.add(std::get<0>(x_min),std::get<1>(x_min),T[std::get<0>(x_min)][std::get<1>(x_min)]);
        narrow.pop();
        narrow.tri();
    }

    auto end_t = std::chrono::high_resolution_clock::now();

    // Calculer la durée écoulée
    std::chrono::duration<double> duration = end_t - start_t;

    // Afficher le temps écoulé
    std::cout << "Temps d'exécution  FMM : " << duration.count() << " secondes." << std::endl;

    return duration.count();
}

