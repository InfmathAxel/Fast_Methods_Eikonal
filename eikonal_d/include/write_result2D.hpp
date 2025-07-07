
#include <sstream>  
#include <fstream>
#include <vector>
#include <iostream>
#include <ctime>
#pragma once 

int write_files(const  std::vector<std::vector<double>> &T ,std::string file){
    

    std::ostringstream filename;
    filename << file << ".txt";

    std::ofstream fichier(filename.str());

    // Vérifier si l'ouverture a réussi
    if (!fichier) {
        std::cerr << "Erreur d'ouverture du fichier !" << std::endl;
        return 1;
    }

    // Écrire la matrice dans le fichier
    for (const auto& ligne : T) {  
        for (const auto& val : ligne) {  
            fichier << val << " ";  
        }
        fichier << std::endl;  
    }

    // Fermer le fichier
    fichier.close();

    std::cout << "La matrice a été écrite dans :" << filename.str() << std::endl;
    return 0;
}

/**
 * @brief fonction pour enregistrer les temps d'execution des methodes en avec les nombres de points de maillage associer  
 */
void saveResults(std::ofstream& file, int n_source, int nb_n ,const int n_tab[], const std::vector<double>& time_fmm,
    const std::vector<double>& time_fsm, const std::vector<double>& time_fim, const std::vector<double>& time_exact) {
    file << n_source << "\n" ;
    for (int i = 0; i < nb_n; i++) 
    {
        file << n_tab[i];
        if (i < nb_n) file << " ";
    }
        file << "\n";

    for (double t : time_fmm) file << t << " ";
    file << "\n";

    for (double t : time_fsm) file << t << " ";
    file << "\n";

    
    for (double t : time_fim) file << t << " ";
    file << "\n";

    for (double t : time_exact) file << t << " ";
    file << "\n" ;


    
}


/**
 * permet d'enregistrer la liste des sources.
 */
void save_Xs(const std::vector<std::pair<int,int>>& Xs, const std::string& file,double steps) {
    std::ostringstream filename;
    filename << file << ".txt";

    std::ofstream fichier(filename.str());
    
    if (!filename) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier " << file << std::endl;
        return;
    }

    for (const auto& src : Xs) {
        fichier << src.second * steps << " " << src.first *steps  << "\n";
    }

    fichier.close();
    std::cout << "Coordonnées sauvegardées dans " << file << std::endl;
}