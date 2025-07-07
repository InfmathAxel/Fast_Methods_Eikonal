#include  <vector>
#include <iostream>

#pragma once 


class source{
    public:
    
    source(std::pair<int,int> Xi,double x,double y ){
        coord_x = x;
        coord_y = y ;
        index = Xi;
    }

    void set_x(double x ){ coord_x = x ;}
    void set_y(double y ){ coord_y = y ;}
    double get_x() const {return coord_x;}
    double get_y() const {return coord_y;}

    void set_index(std::pair<int,int> ind){index = ind;}
    std::pair<int,int> get_ind(){return index;}

    void print(){
        std::cout << "index (" << index.first << "," << index.second << ") coords = ( " << coord_x << "," << coord_y << ")" << std::endl; 
    }
    
    private:

    std::pair<int,int> index;
    double coord_x;
    double coord_y;

};

void save(const std::vector<source>& Xs, const std::string& filename) {
    std::ofstream file(filename);
    
    if (!file) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier " << filename << std::endl;
        return;
    }

    for (const auto& src : Xs) {
        file << src.get_y() << " " << src.get_x() << "\n";
    }

    file.close();
    std::cout << "Coordonnées sauvegardées dans " << filename << std::endl;
}