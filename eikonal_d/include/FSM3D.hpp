#include "sweep3D.hpp"


int all_converge(std::vector<std::vector<std::vector<double>>> &T,int n )
    {   bool stop = true;
        int compteur = 0 ;
        for(int i = 0 ; i < n ; i++) {
            for(int j= 0 ; j < n ; j ++){
                for (int k = 0 ; k < n ; k++){
                    if (T[i][j][k] == 0 ){
                        compteur ++ ;
                    }
                        
                }
            }
        }
        std::cout << "nb_non_converge : " << compteur << std::endl;
        return compteur;
    }
double FSM3D(std::vector<std::vector<std::vector<double>>> &T,int n,double lenght,const std::vector<std::tuple<int,int,int>> &Xs,std::vector<std::vector<std::vector<double>>> &Closest,std::vector<std::vector<std::vector<double>>> &Lock )
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
    std::vector<std::vector<std::vector<double>>> F(n, std::vector<std::vector<double>>(n, std::vector<double>(n, 1.0)));
    double compteur_Xs = 1;
    
    for (auto xi : Xs)
    {   int i = std::get<0>(xi);
        int j = std::get<1>(xi);
        int k = std::get<2>(xi);
        T[i][j][k] = 0 ;
        Closest[i][j][k] = compteur_Xs;
        Lock[i][j][k] = 1 ;
        compteur_Xs++;
    }

    int N = 3 ;
    int a ;
    int* sweep_direction = new int[3] {1, 1, 1};
    bool stop = false;
    int compteur = 0 ; 
    int new_compteur;
    // du tant que l'on n'a pas sweep dans toutes les directions on itère: 
    while (!stop) {
        getSweepDirection(N,sweep_direction);
        std::cout << "direction " << sweep_direction[0] << " " << sweep_direction[1] << sweep_direction[2]<< std::endl;
        stop = Sweep(T,Closest,Lock,F,sweep_direction,N,n,m,h);
        
       
    }
    delete[] sweep_direction;

    auto end_t = std::chrono::high_resolution_clock::now();

    // Calculer la durée écoulée
    std::chrono::duration<double> duration = end_t - start_t;

    // Afficher le temps écoulé
    std::cout << "Temps d'exécution FSM : " << duration.count() << " secondes." << std::endl;

    return duration.count();


}