
#include "SolveEikonal3D.hpp"

#include <unordered_set>

// hash function to stock std::pair in unordered set

template <>
struct std::hash<std::tuple<int, int, int>> {
    size_t operator()(const std::tuple<int, int, int>& triplet) const {
        return std::hash<int>()(std::get<0>(triplet)) ^
               std::hash<int>()(std::get<1>(triplet)) ^
               std::hash<int>()(std::get<2>(triplet));
    }
};

double FIM3D(std::vector<std::vector<std::vector<double>>> &T,int n,double lenght,const std::vector<std::tuple<int,int,int>> &Xs,std::vector<std::vector<std::vector<double>>> &Closest )
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

    std::vector<std::vector<std::vector<double>>> F(n, std::vector<std::vector<double>>(n, std::vector<double>(n, 1.0)));
    double compteur_xs = 1 ;
    // unordered set to have easy acess to the pair of index and easy acess to find method
    std::unordered_set<std::tuple<int, int, int>> narrow;
    std::unordered_set<std::tuple<int, int, int>> frozen;


    for (auto xi : Xs)
    {   int i = std::get<0>(xi);
        int j = std::get<1>(xi);
        int k = std::get<2>(xi);
        T[i][j][k] = 0 ;
        Closest[i][j][k] = compteur_xs;
        compteur_xs++;
        frozen.insert({i,j,k});
    }


    
   

    // stock all neighbord of source in
    for (const auto &pair : Xs)
    {
        std::vector<std::tuple<int,int,int>> direction = {{1,0,0},{0,1,0},{0,0,1},{-1,0,0},{0,-1,0},{0,0,-1}};
        for (auto& direc : direction)
        {
            int di = std::get<0>(direc) + std::get<0>(pair);
            int dj = std::get<1>(direc) + std::get<1>(pair);
            int dk = std::get<2>(direc) + std::get<2>(pair);

            if ((0 <= di ) && (di < n ) && (0 <= dj) && (dj < m ) && ((0 <= dk) && (dk < m ))) {
                narrow.insert({di,dj,dk});
                Closest[di][dj][dk] = Closest[std::get<0>(pair)][std::get<1>(pair)][std::get<1>(pair)];
            }
        }
    }

    double eps = 1e-5;
    std::array<int,3> close ;
    // boucle on narrow set
    int iter = 0 ;
    int new_size = -1 ;
    int old_size = 0 ; 
    while (!narrow.empty())
    {   
        std::cout << narrow.size() <<  std::endl ;
        new_size =  narrow.size();
        std::unordered_set<std::tuple<int,int,int>>  copie_narrow(narrow);
        //std::cout << old_size  << " " << new_size << std::endl;
        for (const auto& pair : copie_narrow)
        {   if (iter %5000 == 0) 
            std::cout << narrow.size() << "iter : " << iter <<  std::endl ;
            new_size =  narrow.size();
            //std::cout <<"copie narrow size : " <<  copie_narrow.size()  << "narrow size : " << narrow.size() << "frozen size : " << frozen.size() << std::endl;
            // on compare la distance stockée dans  T et la distance calculé avec SolveEikonale
            int xi[3] = {std::get<0>(pair),std::get<1>(pair),std::get<2>(pair)};
            double ti = T[xi[0]][xi[1]][xi[2]];
            double Ti_tild = SolveEikonale3D(xi, T, F, n, h,close);
            T[xi[0]][xi[1]][xi[2]] = Ti_tild;
            Closest[xi[0]][xi[1]][xi[2]] = Closest[close[0]][close[1]][close[2]];
            // std::cout << "comparaison " << std::endl;
            // Si le seuil est dépassé on calcule une première fois T pour les voisins de xi et on les stock dans narrows
            if (std::abs(Ti_tild-ti) < eps || old_size == new_size)
            {   // on cherche les voisins des xi
                std::vector<std::tuple<int,int,int>> x_neighboor;
                std::vector<std::tuple<int,int,int>> direction = {{1,0,0},{0,1,0},{0,0,1},{-1,0,0},{0,-1,0},{0,0,-1}};
                for (auto& direc : direction)
                {
                    int di = std::get<0>(direc) + std::get<0>(pair);
                    int dj = std::get<1>(direc) + std::get<1>(pair);
                    int dk = std::get<2>(direc) + std::get<2>(pair);

                    if ((0 <= di ) && (di < n ) && (0 <= dj) && (dj < m ) && ((0 <= dk) && (dk < m ))) {
                        x_neighboor.push_back({di,dj,dk});

                    }
                }

                // std::cout << "voisin trouvé " << std::endl ;
                for (auto& x_nb : x_neighboor)
                {   // pour tout les voisins on calcule T et on ajoute xi à narrow si le points n'y est pas déjà
                    int x = std::get<0>(x_nb);
                    int y = std::get<1>(x_nb);
                    int z = std::get<2>(x_nb);
            
                    if ((copie_narrow.find({x,y,z})== copie_narrow.end()) )
                    {   int xj[3] = {x,y,z};
                        double p = T[x][y][z];
                        double q = SolveEikonale3D(xj, T, F, n, h,close);
                        if (p > q)
                        {
                            T[xj[0]][xj[1]][xj[2]] = q;

                            Closest[xj[0]][xj[1]] = Closest[xi[0]][xi[1]];

                            narrow.insert({xj[0],xj[1],xj[2]});

                        }
                    }
                    
                }
                // std::cout << "erase pair de narrow " << std::endl ;
                narrow.erase(pair);
            
            }
            iter++;


        }
        
    }

    auto end_t = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_t - start_t;

// Afficher le temps écoulé
    std::cout << "Temps d'exécution FIM : " << duration.count() << " secondes." << std::endl;
    return duration.count();
}