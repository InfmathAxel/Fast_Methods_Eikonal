#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>

#pragma once

class Unknow
/**
 * @brief Class Unknow de la méthode FMM
 *
 * S'initialise vide puis sera manipuler dans le code de la FMM
 * - unknow : map contenant l'indice des points dans un std::pair<int,int>  et leur distance associer
 * - nb_elem : nombre d'éléments dans unknow
 *
 * méthode importante de la classe :
 * check(i,j) vérifie si le point d'indice (i,j) est dans la map
 * add(i,j,distance) qui rajoute ({i,j},T) dans la map
 * remove(i,j) qui enlève le point d'indice (i,j) de la map
 */
{

private:
    int nb_elem;
    std::map<std::pair<int, int>, double> unknow;

public:
    Unknow()
    {
        nb_elem = 0 ;
    }
    void add(int x,int y,double T)
    {
        unknow[ {x, y}] = T;
        nb_elem++;
    }

    void display() const
    {
        for (const auto& pair : unknow)
        {
            std::cout << "Clé: (" << pair.first.first << ", " << pair.first.second
                      << ") -> Valeur: " << pair.second << std::endl;
        }
    }
    bool check(int x, int y) {
        return unknow.find({x, y}) != unknow.end();
    }

    void remove(int x,int y )
    {
        unknow.erase({x,y});
    }

};



class Narrow
{
    /**
     * @brief Class Narrow de la méthode FMM
     *
     * S'initialise vide puis sera manipuler dans le code de la FMM
     * - narrow : vecteur  contenant l'indice des points dans un std::pair<int,int>  et leur distance associer ({i,j},T)
     * - nb_elem : nombre d'éléments dans narrow
     *
     * méthode importante de la classe :
     * check(i,j) vérifie si le point d'indice (i,j) est dans le vecteur
     * push(i,j,distance) qui rajoute ({i,j},T) dans la vecteur au bout
     * pop() qui enlève le premier élément du vecteur
     * top() retourne le premier élément du vecteur
     * increase(i,j,T) modifie la valeur T du point d'indice (i,j)
     * tri() utilise std::sort pour trier la liste dans l'ordre croissant en fonction de T ,tri en O(n log(n)) n le nb_élément
     */
private:
    std::vector<std::tuple<int,int,double>> narrow;
    int nb_elem;


public:
    Narrow()
    {
        nb_elem = 0;
    }

    int get_elem()
    {
        return nb_elem;
    }

    void push(int x,int y,double T)
    {
        narrow.push_back({x,y,T});
        nb_elem++;
    }

    void pop()
    {
        if (!narrow.empty())
        {
            narrow.erase(narrow.begin());
            nb_elem--;
        }
        else
        {
            std::cout<< "liste narrow vide " << std::endl ;
        }
    }

    std::tuple<int,int,double> top()
    {
        return narrow[0];
    }


    void increase(int x, int y, double T)
    {
        for (auto& elem : narrow) {
            if (std::get<0>(elem) == x && std::get<1>(elem) == y)
            {
                std::get<2>(elem) = T;  // Modifie la valeur T du tuple trouvé
                break;  // On a trouvé et modifié l'élément, on peut sortir de la boucle
            }
        }

    }

    void tri()
    {
        std::sort(narrow.begin(), narrow.end(), [](const auto& a, const auto& b)
        {
            return std::get<2>(a) < std::get<2>(b);  // Trie par ordre croissant de T avec std::sort et un fonction de tri std::get(2) donnant la valeur de T
        }
                 );
    }

    bool check(int x,int y)
    {
        bool find = false;
        for (auto& elem : narrow)
        {
            if (std::get<0>(elem) == x && std::get<1>(elem) == y)
            {
                find = true;
                break;
            }
        }
        return find;
    }

    void display() const
    {
        std::cout << "Éléments dans Narrow :" << std::endl;
        for (const auto& elem : narrow) {
            std::cout << "(" << std::get<0>(elem) << ", "
                      << std::get<1>(elem) << ", "
                      << std::get<2>(elem) << ")" << std::endl;
        }
    }

};



class Frozen
/**
 * @brief Class Frozen de la méthode FMM
 *
 * S'initialise vide puis sera manipuler dans le code de la FMM
 * - frozen : map contenant l'indice des points dans un std::pair<int,int> et T qui ont convergé.
 * - nb_elem : nombre d'éléments dans frozen
 *
 * méthode important de la classe :
 * -add(i,j,T) qui rajoute T dans la clé (i,j)
 * -check(i,j) qui vérifie si (i;j) est dans la map
 *
 */
{
private:
    int nb_elem;
    std::map<std::pair<int,int>,double > frozen;

public:
    Frozen()
    {
        nb_elem = 0 ;
    }


    void add(int x,int y, double T)
    {
        frozen[ {x,y}] = T;
        nb_elem ++;
    }

    void display() const
    {
        std::cout << "for frozen " ;
        for (const auto& pair : frozen)
        {
            std::cout << "Clé: (" << pair.first.first << ", " << pair.first.second
                      << ") -> Valeur: " << pair.second << std::endl;
        }
    }

    bool check(int x, int y)
    {
        return frozen.find({x, y}) != frozen.end();
    }

};
