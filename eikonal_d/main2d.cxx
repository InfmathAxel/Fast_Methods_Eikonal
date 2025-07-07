#include <sstream>  // Pour construire la chaîne formatée
#include <fstream>
#include <random>
#include "include/FSM2D.hpp"
#include "include/FIM2D.hpp"
#include "include/FMM2D.hpp"
#include "include/random_source2D.hpp"
#include "include/write_result2D.hpp"
#include "include/euclidien2D.hpp"



/**
 * Dans ce main on test nos méthodes en 2D séquentielles.
 */
int main(int argc, char* argv[])

{   int n_source = 0;
    int nb_n_source = 0 ;
    int *n_source_tab = nullptr; 
    int n = 0;
    
    if (argc == 3) {
        n_source = std::stoi(argv[1]);
        n = std::stoi(argv[2]);  
        std::cout << "Valeur de n_source et n  : " << n_source <<" " << n <<  std::endl;
        if (n_source > n ) {
            std::cout << "usage : n_source < n " << std::endl;
            return 1 ;
        }
    }
    else 
    {
        std::cout << "Usage: " << argv[0] << " <n_source>  <liste_source>" << std::endl;
        return 1;
    }
     

    double length = 10 ;
    std::vector<std::vector<double>> T_FMM(n, std::vector<double>(n, std::numeric_limits<double>::infinity()));
    std::vector<std::vector<double>> closest_FMM(n,std::vector<double>(n,0));
    std::vector<std::vector<double>> T_FIM(n, std::vector<double>(n, std::numeric_limits<double>::infinity()));
    std::vector<std::vector<double>> closest_FIM(n,std::vector<double>(n,0));
    std::vector<std::vector<double>> T_exact(n, std::vector<double>(n, std::numeric_limits<double>::infinity()));
    std::vector<std::vector<double>> closest_exact(n,std::vector<double>(n,0));
    std::vector<std::vector<double>> closest_FSM(n,std::vector<double>(n,0));
    std::vector<std::vector<double>> T_FSM(n, std::vector<double>(n, std::numeric_limits<double>::infinity()));
    std::vector<std::pair<int,int>> Xs;

    bool give_seed = true;
    int seed = 1234;
    generate_source_sinus2d(Xs,n,seed,true);
    //generate_source2d(Xs,n,n_source,seed,true);

    //FMM(T_FMM,n,length,Xs,closest_FMM);
    FSM2D(T_FSM,n,length,Xs,closest_FSM);
    FIM2D_seuil(T_FIM,n,length,Xs,2,closest_FIM);
    euclidien2d(T_exact,Xs,n,n,length,closest_exact);


    std::ostringstream path_fmm, path_fsm, path_fim, path_exact;

    path_fmm << "results/length_" << length << "_n_" << n << "_n_source_" << n_source << "_FMM";
    path_fsm << "results/length_" << length << "_n_" << n << "_n_source_" << n_source << "_FSM";
    path_fim << "results/length_" << length << "_n_" << n << "_n_source_" << n_source << "_FIM";
    path_exact << "results/length_" << length << "_n_" << n << "_n_source_" << n_source << "_exact";

    std::string tag_fmm = path_fmm.str();
    std::string tag_fsm = path_fsm.str();
    std::string tag_fim = path_fim.str();
    std::string tag_exact = path_exact.str();
    write_files(T_FIM,tag_fim);
    write_files(T_FMM,tag_fmm);
    write_files(T_FSM,tag_fsm);
    write_files(T_exact,tag_exact);
    //write_files(closest_exact,tag_exact);
    //write_files(closest_FIM,tag_fim);
    //write_files(closest_FMM,tag_fmm);
    //write_files(closest_FSM,tag_fsm);
    save_Xs(Xs,"coords_mesh",length/(n-1));

    return 0;
    }

    