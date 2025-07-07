#include "SolveEikonal2D.hpp"
#include "database.hpp"
#include <unordered_set>


//
#pragma once

double FMM(std::vector<std::vector<double>> &T,int n,double lenght,const std::vector<std::pair<int,int>> &Xs, std::vector<std::vector<double>> &Closest)
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

    double compteur_xs = 1;
    for (auto xi : Xs)
    {   int i = std::get<0>(xi);
        int j = std::get<1>(xi);
        narrow.push(i,j,0);
        unknow.remove(i,j);
        T[i][j] = 0 ;
        Closest[i][j] = compteur_xs;
        compteur_xs++;
    }
    //print(Closest);

    std::pair<int,int> close;
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
                ti = SolveEikonale2D(xi,T,F,n,h,close);
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

