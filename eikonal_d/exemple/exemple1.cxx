#include <sstream>  // Pour construire la chaîne formatée
#include <fstream>
#include <random>
#include "../include/FSM2D.hpp"
#include "../include/FIM2D.hpp"
#include "../include/FMM2D.hpp"
#include "../include/random_source2D.hpp"
#include "../include/write_result2D.hpp"
#include "../include/euclidien2D.hpp"

int main()
{
    //initialisation des données 
    int n = 400;
    int nb_source = 4;
    double length = 10;

    //initialisation matrice de distance pour la FIM et FSM

    std::vector<std::vector<double>> T_FSM(n, std::vector<double>(n, std::numeric_limits<double>::infinity()));
    std::vector<std::vector<double>> T_FIM(n, std::vector<double>(n, std::numeric_limits<double>::infinity()));

    std::vector<std::vector<double>> closest_FSM(n,std::vector<double>(n,0));
    std::vector<std::vector<double>> closest_FIM(n,std::vector<double>(n,0));

    //initialisation des Xs a la main : 
    std::vector<std::pair<int,int>> Xs;
    Xs.push_back({100,100});
    Xs.push_back({100,300});
    Xs.push_back({300,100});
    Xs.push_back({300,300});

    //Appelle de la FIM et FSM

    FIM2D(T_FIM,n,length,Xs,closest_FIM);
    FSM2D(T_FSM,n,length,Xs,closest_FSM);

    //écriture des fichiers résultat

    write_files(T_FIM,"resultat_exemple1/FIM_exemple1");
    write_files(T_FIM,"resultat_exemple1/FSM_exemple1");
    save_Xs(Xs,"resultat_exemple1/Xs",length/(n-1));
    
    std::cout << "On a généré deux fichiers txt contenant les résultats. On peut maintenant faire la visualisation avec le code python 'visu_exemple1.py' "  << std::endl;
    return 0;
}