
#include "SolveEikonal2D.hpp"

#include <unordered_set>

// hash function to stock std::pair in unordered set
struct pair_hash 
/**
 * stucture de hashage pour stocker des std::pair<int,int> dans un unordered set 
 */
{
    std::size_t operator()(const std::pair<int, int>& p) const {
        return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
    }
};


double FIM2D(std::vector<std::vector<double>> &T,int n,double lenght,const std::vector<std::pair<int,int>> &Xs,std::vector<std::vector<double>> &Closest )
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
    {   int i = std::get<0>(xi);
        int j = std::get<1>(xi);
        T[i][j] = 0 ; 
        Closest[i][j] = compteur_xs;
        compteur_xs++;
    }
    

    // unordered set to have easy acess to the pair of index and easy acess to find method
    std::unordered_set<std::pair<int,int>, pair_hash> narrow;
    
    // stock all neighbord of source in
    for (const auto &pair : Xs)
    {
        std::vector<std::pair<int,int>> direction = {{-1,0},{1,0},{0,-1},{0,1}};
        for (auto& direc : direction)
        {
            int di = direc.first + std::get<0>(pair);
            int dj = direc.second + std::get<1>(pair);

            if ((0 <= di ) && (di < n ) && (0 <= dj) && (dj < m )){
                narrow.insert({di,dj});
                Closest[di][dj] = Closest[std::get<0>(pair)][std::get<1>(pair)];
        }}
    }
    
    double eps = 1e-2;
    int k ;
    std::pair<int,int> close ;
    // boucle on narrow set
    while (!narrow.empty())
    {
        std::unordered_set<std::pair<int,int>, pair_hash>  copie_narrow(narrow);
        for (const auto& pair : copie_narrow)
        {   
            // on compare la distance stockée dans T et la distance calculé avec SolveEikonale 
            int xi[2] = {std::get<0>(pair),std::get<1>(pair)};
            double ti = T[xi[0]][xi[1]];
            double Ti_tild = SolveEikonale2D(xi, T, F, n, h,close);
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
                        double q = SolveEikonale2D(xj, T, F, n, h,close);
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



double FIM2D_seuil(std::vector<std::vector<double>> &T,int n,double lenght,const std::vector<std::pair<int,int>> &Xs,double max_dist,std::vector<std::vector<double>> &Closest )
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
    {   int i = std::get<0>(xi);
        int j = std::get<1>(xi);
        T[i][j] = 0 ; 
        Closest[i][j] = compteur_xs;
        compteur_xs++;
    }
    

    // unordered set to have easy acess to the pair of index and easy acess to find method
    std::unordered_set<std::pair<int,int>, pair_hash> narrow;
    
    // stock all neighbord of source in
    for (const auto &pair : Xs)
    {
        std::vector<std::pair<int,int>> direction = {{-1,0},{1,0},{0,-1},{0,1}};
        for (auto& direc : direction)
        {
            int di = direc.first + std::get<0>(pair);
            int dj = direc.second + std::get<1>(pair);

            if ((0 <= di ) && (di < n ) && (0 <= dj) && (dj < m )){
                narrow.insert({di,dj});
                Closest[di][dj] = Closest[std::get<0>(pair)][std::get<1>(pair)];
        }}
    }
    
    double eps = 1e-2;
    int k ;
    std::pair<int,int> close ;
    // boucle on narrow set
    while (!narrow.empty())
    {
        std::unordered_set<std::pair<int,int>, pair_hash>  copie_narrow(narrow);
        for (const auto& pair : copie_narrow)
        {   
            // on compare la distance stockée dans T et la distance calculé avec SolveEikonale 
            int xi[2] = {std::get<0>(pair),std::get<1>(pair)};
            double ti = T[xi[0]][xi[1]];
            double Ti_tild = SolveEikonale2D(xi, T, F, n, h,close);
            
                T[xi[0]][xi[1]] = Ti_tild;
            Closest[xi[0]][xi[1]] = Closest[std::get<0>(close)][std::get<1>(close)];
            // Si le seuil est dépassé on calcule une première fois T pour les voisins de xi et on les stock dans narrows
            if (std::abs(Ti_tild-ti) < eps)
            {   
                if(Ti_tild < max_dist){

                
                // on cherche les voisins des xi 
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
                        double q = SolveEikonale2D(xj, T, F, n, h,close);
                        if (p > q)
                        {
                            T[xj[0]][xj[1]] = q;
                            
                            Closest[xj[0]][xj[1]] = Closest[xi[0]][xi[1]];
                           
                            narrow.insert({xj[0],xj[1]});
                            
                        }
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