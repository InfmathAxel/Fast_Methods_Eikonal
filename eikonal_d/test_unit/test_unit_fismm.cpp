#include "../include/FSM2D.hpp"
#include "../include/FIM2D.hpp"
#include "../include/FMM2D.hpp"
#include "../include/euclidien2D.hpp"
#include <limits>
#include <algorithm>
#include <iostream>


int main(){
    int n = 100;
    double lenght = 1;
    int n_source = 2 ;

    std::vector<std::vector<double>> T_FMM(n, std::vector<double>(n, std::numeric_limits<double>::infinity()));
    std::vector<std::vector<double>> closest_FMM(n,std::vector<double>(n,0));

    std::vector<std::vector<double>> T_FIM(n, std::vector<double>(n, std::numeric_limits<double>::infinity()));
    std::vector<std::vector<double>> closest_FIM(n,std::vector<double>(n,0));

    std::vector<std::vector<double>> T_FSM(n, std::vector<double>(n, std::numeric_limits<double>::infinity()));
    std::vector<std::vector<double>> closest_FSM(n,std::vector<double>(n,0));

    std::vector<std::vector<double>> T_exact(n, std::vector<double>(n, std::numeric_limits<double>::infinity()));
    std::vector<std::vector<double>> closest_exact(n,std::vector<double>(n,0));
    std::vector<std::pair<int,int>> Xs;

    Xs.push_back({0,0});
    Xs.push_back({80,60});

    FMM(T_FMM,n,lenght,Xs,closest_FMM);
    FIM2D(T_FIM,n,lenght,Xs,closest_FIM);
    FSM2D(T_FSM,n,lenght,Xs,closest_FSM);
    euclidien(T_exact,Xs,n,n,lenght,closest_exact);

    double eps = 0.05;

    bool fmm = true;
    bool fim = true;
    bool fsm = true;

    for(int i = 0 ; i < n ; i++){
        for(int j= 0 ; j < n ; j++){
            if (std::abs(T_FMM[i][j]-T_exact[i][j]) > eps){
                fmm = false;
            }
            if (std::abs(T_FIM[i][j]-T_exact[i][j]) > eps){
                fim = false;
            }
            
            if (std::abs(T_FSM[i][j]-T_exact[i][j]) > eps){
                fsm = false;
            }
            
        }
    }

    
    if (!fmm)
        std::cout << "probleme valeur fmm " << std::endl;
    if (!fsm)
        std::cout << "probleme valeur fsm " << std::endl;
    if (!fim)
        std::cout << "probleme valeur fim " << std::endl;

    if(fsm && fim && fmm)
        std::cout << "convergence valeur ok !" << std::endl;


    int compteur_faux_fmm = 0 ;
    int compteur_faux_fsm = 0 ;
    int compteur_faux_fim = 0 ;
    for(int i = 0 ; i < n ; i++){
        for(int j= 0 ; j < n ; j++){ 
            if (closest_FMM[i][j] =! closest_exact[i][j] ){
                compteur_faux_fmm ++;
            }
            if (closest_FIM[i][j] =! closest_exact[i][j] ){
                compteur_faux_fim ++;
            }
            
            if (closest_FSM[i][j] =! closest_exact[i][j]){
                compteur_faux_fsm ++;
            }

        }
    }  

    double nb_cellule_tot = n*n;
    fmm = (compteur_faux_fmm/nb_cellule_tot >0.05 );
    fim = (compteur_faux_fim/nb_cellule_tot >0.05 );
    fsm = (compteur_faux_fsm/nb_cellule_tot >0.05) ;

    if (fmm )
        std::cout << "probleme  closest fim " << compteur_faux_fim/nb_cellule_tot << std::endl;
    if (fim )
        std::cout << "probleme  closest fmm " << compteur_faux_fmm/nb_cellule_tot << std::endl;
    if (fsm )
        std::cout << "probleme  closest fsm " <<compteur_faux_fsm/nb_cellule_tot <<  std::endl;
    if(!fmm && !fsm && !fim)
        std::cout << "closest ok ! " << std::endl;
    
    
    return 0 ;
}
