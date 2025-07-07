#include <vector>
#include <random>
#include <ctime>
#pragma once 

void generate_source3d(std::vector<std::tuple<int,int,int>> &Xs,int n,int n_source, unsigned int seed = 0 , bool give_seed = false, bool save_seed = false )
/**
 *  @brief génére les interfaces aléatoirement dans Xs et save la seed au besoin 
 *   
 * - Xs : vecteur qui stockera l'indice des interfaces
 * - n nombre : de point selon les axe x,y 
 * - n_source : nombre d'interfaces à générer 
 * - seed : graine de génération 
 * - give seed : précise si on donne une graine ou non 
 * - save_seed : précise si on veut enregistrer la graine dans un fichier  
*/

{ 
    unsigned int actual_seed;
    
    if (!give_seed) {
        std::random_device rd;
        actual_seed = rd();  
    } else {
        actual_seed = seed; 
    }

    std::mt19937 gen(actual_seed);

    std::uniform_int_distribution<int> dist_x(0, n - 1);
    std::uniform_int_distribution<int> dist_y(0, n - 1);
    std::uniform_int_distribution<int> dist_z(0, n - 1);

    for(int i = 0 ; i < n_source ; i++){
        int x = dist_x(gen);
        int y = dist_y(gen);
        int z = dist_z(gen);
        Xs.push_back({x,y,z});
    
    }

    if (save_seed == true){
        // Obtenir l'heure et la date actuelles
        std::time_t t = std::time(nullptr);
        std::tm* now = std::localtime(&t);

        // Formater la date et l'heure (YYYY-MM-DD_HH-MM-SS)
        std::ostringstream datetime;
        datetime << (now->tm_year + 1900) << "-"
                 << (now->tm_mon + 1) << "-"
                 << now->tm_mday << "_"
                 << now->tm_hour << "-"
                 << now->tm_min << "-"
                 << now->tm_sec;

        std::ostringstream filename;
        filename << "results/seed_of_" << n 
                 << "_n_source_" << n_source
                 << "_" << datetime.str() << ".txt";


        std::ofstream seed_file(filename.str());
        seed_file << seed;
        seed_file.close();
    }
}