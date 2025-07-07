
#include <sstream>  
#include <fstream>
#include <vector>
#include <iostream>
#pragma once 
int write_files(const  std::vector<std::vector<double>> &T,std::string tag){
    std::ostringstream filename;
    filename << tag << ".txt";

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


#include <sys/stat.h> // Pour mkdir
#include <dirent.h>   // Pour opendir, readdir, closedir
#include <unistd.h>   // Pour rmdir


void clearDirectory(const std::string& dirPath) {
    DIR* dir = opendir(dirPath.c_str());
    if (!dir) {
        std::cerr << "Erreur : Impossible d'ouvrir le dossier " << dirPath << std::endl;
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_type == DT_REG || entry->d_type == DT_UNKNOWN) {
            std::string filePath = dirPath + "/" + entry->d_name;
            if (remove(filePath.c_str()) != 0) {
                std::cerr << "Erreur : Impossible de supprimer le fichier " << filePath << std::endl;
            }
        }
    }
    closedir(dir);
}



void writeVTKFile2D(const std::string& tag, const std::vector<std::vector<double>>& T, double h, int k) {
    // Créer le chemin du dossier en incluant le tag
    std::string dirPath = "/ccc/work/cont001/ocre/demutha/results/" + tag;

   

    // Créer le chemin du fichier
    std::ostringstream filename;
    filename << dirPath << "/" << tag << "_" << k << ".vtk";

    std::ofstream file(filename.str());
    if (!file.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier " << filename.str() << std::endl;
        return;
    }

    int nx = T.size();
    int ny = T[0].size();

    file << "# vtk DataFile Version 3.0\n";
    file << "2D Matrix Data\n";
    file << "ASCII\n";
    file << "DATASET STRUCTURED_POINTS\n";
    file << "DIMENSIONS " << nx << " " << ny << " 1\n";
    file << "ORIGIN 0 0 0\n";
    file << "SPACING " << h << " " << h << " 1\n";
    file << "POINT_DATA " << nx * ny << "\n";
    file << "SCALARS T double\n";
    file << "LOOKUP_TABLE default\n";

    int compteur = 0;
    for (int j = 0; j < ny; ++j) {
        for (int i = 0; i < nx; ++i) {
            double value = T[i][j];
            if (std::isnan(value) || std::isinf(value)) {
                value = 0.0; // Remplacer par une valeur par défaut ou gérer l'erreur
                compteur++;
            }
            file << value << " ";
        }
        file << "\n";
    }

    file.close();
    std::cout << "inf : " << compteur << std::endl;
    std::cout << "Fichier VTK sauvegardé sous " << filename.str() << std::endl;
}


int write_files_temps(const  std::vector<std::vector<double>> &T ,std::string tag){
    std::ostringstream filename;
    filename <<  tag << ".txt";

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

int write_files_temps2(const  std::vector<double> &T ,std::string tag){
    std::ostringstream filename;
    filename <<  tag << ".txt";

    std::ofstream fichier(filename.str());

    // Vérifier si l'ouverture a réussi
    if (!fichier) {
        std::cerr << "Erreur d'ouverture du fichier !" << std::endl;
        return 1;
    }

    // Écrire la matrice dans le fichier
    for (const auto& val : T) {
        fichier << val << " ";
    }

    // Ajouter une nouvelle ligne à la fin du fichier
    fichier << "\n";


    // Fermer le fichier
    fichier.close();

    std::cout << "La matrice a été écrite dans :" << filename.str() << std::endl;
    return 0;
}
