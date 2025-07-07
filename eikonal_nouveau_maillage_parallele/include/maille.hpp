#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <algorithm>
#pragma once


double eucli(const coords &point_1 ,const coords &point_2){
    double dx = point_1.get_x() - point_2.get_x();
    double dy = point_1.get_y() - point_2.get_y();
    return std::sqrt(dx * dx + dy * dy);
}


class coords {
/**
 * 
 */
public: 
    coords(double coord_x = 0, double coord_y = 0) : x(coord_x), y(coord_y) {}

    double get_x() const { return x; }
    double get_y() const { return y; }

    friend std::ostream& operator<<(std::ostream& os, const coords& c) {
        os << "(" << c.x << ", " << c.y << ")";
        return os;
    }

private:
    double x, y;
};

class meshes {
public:
    meshes(int n_, int m_, double lenghts_, double steps_,unsigned int seed  =  0,bool give_seed = false )
        : n(n_), m(m_), lenghts(lenghts_), steps(steps_),
          coord_noeuds(n, std::vector<std::vector<coords>>(m, std::vector<coords>(4, coords(0,0))))
    {
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < m - 1; j++) {
                coord_noeuds[i][j][0] = coords(i * steps, j * steps);
                coord_noeuds[i][j][1] = coords((i + 1) * steps, j * steps);
                coord_noeuds[i][j][2] = coords((i + 1) * steps, (j + 1) * steps);
                coord_noeuds[i][j][3] = coords(i * steps, (j + 1) * steps);
            }
        }

        unsigned int actual_seed;
    
        if (!give_seed) {
            std::random_device rd;
            actual_seed = rd();  
        } else {
            actual_seed = seed; 
        }
    
        std::mt19937 gen(actual_seed);

        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < m - 1; j++) {
                // Générer des coordonnées aléatoires à l'intérieur de la maille
                std::vector<double> x_coords = { 
                    coord_noeuds[i][j][0].get_x(), 
                    coord_noeuds[i][j][1].get_x(), 
                    coord_noeuds[i][j][2].get_x(), 
                    coord_noeuds[i][j][3].get_x()
                };
                
                double min_x = *std::min_element(x_coords.begin(), x_coords.end());
                double max_x = *std::max_element(x_coords.begin(), x_coords.end());

                std::vector<double> y_coords = { 
                    coord_noeuds[i][j][0].get_y(), 
                    coord_noeuds[i][j][1].get_y(), 
                    coord_noeuds[i][j][2].get_y(), 
                    coord_noeuds[i][j][3].get_y()
                };
                
                double min_y = *std::min_element(y_coords.begin(), y_coords.end());
                double max_y = *std::max_element(y_coords.begin(), y_coords.end());
                
                coords coord((max_x+min_x)/2,(min_y+max_y)/2);
                coords_maille.emplace_back(coord);
            }
        }
    }

    void print() {
        std::cout << "Noeuds = " << std::endl; 
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < m - 1; j++) {
                std::cout << "Maille (" << i << ", " << j << ") : "
                          << coord_noeuds[i][j][0] << ", "
                          << coord_noeuds[i][j][1] << ", "
                          << coord_noeuds[i][j][2] << ", "
                          << coord_noeuds[i][j][3] << std::endl;
            }
        }
        std::cout << "coords de chaque maille =  "  << std::endl;
        int index = 0;
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < m - 1; j++) {
                std::cout << "Maille (" << i << ", " << j << ") coords : " 
                          << coords_maille[index].get_x()  << ", " 
                          << coords_maille[index].get_y() << std::endl;
                index++;
            }
        }
    }


    coords get_coord(int x , int y) const {
        return coords_maille[x*(m-1)+y];
    }

    std::vector<coords> get_maille(int i , int j ){
            return coord_noeuds[i][j]   ;
    }
    void save(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file) {
            std::cerr << "Erreur : Impossible d'ouvrir le fichier " << filename << std::endl;
            return;
        }

        int index = 0;
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < m - 1; j++) {
                
                    file << coords_maille[index].get_x() << " " << coords_maille[index].get_y() << "\n";
                index++;
            }
        }
        file.close();
        std::cout << "Coordonnées sauvegardées dans " << filename << std::endl;
    }



private:
    int n, m; // nb noeuds x y 
    double lenghts, steps; // longueur du maillage et le pas 
    std::vector<std::vector<std::vector<coords>>> coord_noeuds;  // Matrice 2D n*m, chaque élément étant un vecteur de 4 coords
    std::vector<coords> coords_maille;
};
