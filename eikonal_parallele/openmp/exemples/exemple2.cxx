#include <sstream>  // Pour construire la chaîne formatée
#include <fstream>
#include <random>
#include "../include/FSM.cpp"
#include "../include/FIM.cpp"

#include "../include/random_source.cpp"
#include "../include/write_result.cpp"
#include "../include/euclidien.cpp"

int main()
{
    //initialisation des données 
    int n = 400;
    int nb_source = 4;
    double length = 10;
    double h = length/(n-1);
    int n_thread = 4;

    //initialisation matrice de distance pour la FIM et FSM

    std::vector<std::vector<double>> T_FSM(n, std::vector<double>(n, std::numeric_limits<double>::infinity()));
    std::vector<std::vector<double>> T_FIM(n, std::vector<double>(n, std::numeric_limits<double>::infinity()));

    std::vector<std::vector<double>> closest_FSM(n,std::vector<double>(n,0));
    std::vector<std::vector<double>> closest_FIM(n,std::vector<double>(n,0));

    //Initialisation de la matrice de vitesse
    std::vector<std::vector<double>> F(n, std::vector<double>(n, 1));

    //initialisation des Xs en onde pour générer une onde aléatoire passer le true en false, ou changer la seed (12345): 
    std::vector<std::pair<int,int>> Xs;
    int seed = 12345;
    generate_source_sinus2d(Xs,n,seed,true);
    
    //Appel de la FIM et FSM

    FIM2D(T_FIM,F,n,h,Xs,n_thread,closest_FIM);
    FSM2D(T_FSM,F,n,h,Xs,n_thread);

    //écriture des fichiers résultats

    write_files(T_FIM,"resultat_exemple2/FIM_exemple2");
    write_files(T_FIM,"resultat_exemple2/FSM_exemple2");
    save_Xs(Xs,"resultat_exemple2/Xs",length/(n-1));

    std::cout << "On a généré deux fichiers txt contenant les résultats. On peut maintenant faire la visualisation avec le code python 'visu_exemple2.py' "  << std::endl;
    return 0;
}