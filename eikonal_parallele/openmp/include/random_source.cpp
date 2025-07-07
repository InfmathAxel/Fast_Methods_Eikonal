#include <vector>
#include <random>
#include <ctime>
#pragma once 

void generate_source2d(std::vector<std::pair<int,int>> &Xs,int n,int n_source, unsigned int seed = 0 , bool give_seed = false, bool save_seed = false )
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

    for(int i = 0 ; i < n_source ; i++){
        int x = dist_x(gen);
        int y = dist_y(gen);
        Xs.push_back({x,y});
    
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


        std::ofstream seed_file("result/seed.txt");
        seed_file << seed;
        seed_file.close();
    }
}


void generate_source_sinus2d(std::vector<std::pair<int,int>> &Xs,int n, unsigned int seed = 0 , bool give_seed = false, bool save_seed = false ){
    /**
 *  @brief génére les interfaces aléatoirement en forme d'onde dans Xs et save la seed au besoin 
 * - Xs : vecteur qui stockera l'indice des interfaces
 * - n nombre : de point selon les axe x,y 
 * - seed : graine de génération 
 * - give seed : précise si on donne une graine ou non 
 * - save_seed : précise si on veut enregistrer la graine dans un fichier  
*/


    unsigned int actual_seed;
    
    if (!give_seed) {
        std::random_device rd;
        actual_seed = rd();  
    } else {
        actual_seed = seed; 
    }

    std::mt19937 gen(actual_seed);

    std::uniform_int_distribution<int> dist_x(1, n - 1);
    
        int x = 1;
        int y = dist_x(gen);
        Xs.push_back({x,y});

    
        std::uniform_int_distribution<int> dist_y1(-1, 1); // Déclaration avant la boucle

        for (int i = 0; i < n - 2; i++) { 
            if (y == 0)
                dist_y1 = std::uniform_int_distribution<int>(0, 1);  // Seulement vers le haut
            else if (y == n - 1)
                dist_y1 = std::uniform_int_distribution<int>(-1, 0); // Seulement vers le bas
            else 
                dist_y1 = std::uniform_int_distribution<int>(-1, 1); // Mouvement libre
        
            x++;
            y += dist_y1(gen);  // Appliquer la nouvelle distribution
            Xs.push_back({x, y});    
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
                 
                 << "_" << datetime.str() << ".txt";


        std::ofstream seed_file(filename.str());
        seed_file << seed;
        seed_file.close();
    }
}