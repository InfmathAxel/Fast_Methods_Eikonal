
#include <sstream>  
#include <fstream>
#include <vector>
#include <iostream>
#include "source.hpp"
#include <ctime>
#pragma once 

int write_files(const  std::vector<std::vector<double>> &T ,double lenght,int n,int n_source ,std::string tag){
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    char dateStr[20];
    std::strftime(dateStr, sizeof(dateStr), "%Y-%m-%d_%H-%M-%S", localTime);

    std::ostringstream filename;
    filename << "results/length_" << lenght << "_n_" << n << "_n_source_" << n_source << "_" << tag << ".txt";

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


void saveResults(std::ofstream& file, int n_source, int nb_n ,const int n_tab[], const std::vector<double>& time_fmm,
    const std::vector<double>& time_fsm, const std::vector<double>& time_fim, const std::vector<double>& time_exact) {
    file << n_source << "\n" ;
    for (int i = 0; i < nb_n; i++) 
    {
        file << n_tab[i];
        if (i < nb_n) file << " ";
    }
        file << "\n";

   
    //for (double t : time_fmm) file << t << " ";
    //file << "\n";

    
    for (double t : time_fsm) file << t << " ";
    file << "\n";

    
    for (double t : time_fim) file << t << " ";
    file << "\n";

    for (double t : time_exact) file << t << " ";
    file << "\n" ;


    
}

int write_closest(const  std::vector<std::vector<double>> &closest ,const std::vector<source> &Xs,double lenght,int n,int n_source ,std::string tag){
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    char dateStr[20];
    std::strftime(dateStr, sizeof(dateStr), "%Y-%m-%d_%H-%M-%S", localTime);

    std::ostringstream filename;
    filename << "results/length_" << lenght << "_n_" << n << "_n_source_" << n_source << "_closest_" << tag << ".txt";

    std::ofstream fichier(filename.str());

    // Vérifier si l'ouverture a réussi
    if (!fichier) {
        std::cerr << "Erreur d'ouverture du fichier !" << std::endl;
        return 1;
    }

    // Écrire la matrice dans le fichier
    for ( auto xi:Xs){
        std::pair<int,int> pair = xi.get_ind();
        fichier << "(" << std::get<0>(pair) << "," <<std::get<1>(pair) << ") ";  
    }

    fichier << std::endl;
    for (const auto& ligne : closest) {  
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