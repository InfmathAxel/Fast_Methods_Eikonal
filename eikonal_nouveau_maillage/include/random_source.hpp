#include <vector>
#include <random>
#include <ctime>
#include "source.hpp"
#include "maille.hpp"
#pragma once 

void generate_source(std::vector<source> &Xs,meshes mesh, int n,int n_source,double steps, unsigned int seed = 0 , bool give_seed = false, bool save_seed = false )
/**
 *  @brief génére les interfaces aléatoirement dans Xs et save la seed au besoin 
 *   
 * - Xs : vecteur qui stockera l'indice des interfaces
 * - n nombre : de point selon les axe x,y 
 * - n_source : nombre d'interfaces à générer 
 * - seed : graine de génération 
 * - give seed : précise si on donne une graine ou non 
 * - save_seed : précise si on veut enregistrer la graine dans un fichier  
 * - mesh : le maillage associé avec des mailles et non des points.
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

    std::uniform_int_distribution<int> index_i(0, n - 2);
    std::uniform_int_distribution<int> index_j(0, n - 2);
 
    
    for(int k = 0 ; k < n_source ; k++){
        int i = index_i(gen);
        int j = index_j(gen);

        std::vector<coords> random_maille = mesh.get_maille(i,j);
        std::vector<double> x_coords = { 
            random_maille[0].get_x(), 
            random_maille[1].get_x(), 
            random_maille[2].get_x(), 
            random_maille[3].get_x()
        };
        
        double min_x = *std::min_element(x_coords.begin(), x_coords.end());
        double max_x = *std::max_element(x_coords.begin(), x_coords.end());
        std::cout << "x "  << min_x << " " << max_x << std::endl; 
        std::vector<double> y_coords = { 
            random_maille[0].get_y(), 
            random_maille[1].get_y(), 
            random_maille[2].get_y(), 
            random_maille[3].get_y()
        };
        
        double min_y = *std::min_element(y_coords.begin(), y_coords.end());
        double max_y = *std::max_element(y_coords.begin(), y_coords.end());
        std::cout << "y "  << min_y << " " << max_y << std::endl; 
        std::uniform_real_distribution<double> dist_x(min_x,max_x);
        std::uniform_real_distribution<double> dist_y(min_y,max_y);
        
        double x = dist_x(gen);
        double y = dist_y(gen);

        source Xi({i,j},x,y);
        Xs.push_back(Xi);



        
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

void generate_source_sinus(std::vector<std::pair<int,int>> &Xs,int n, unsigned int seed = 0 , bool give_seed = false, bool save_seed = false ){
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