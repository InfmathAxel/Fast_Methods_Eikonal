#include "include/FIM3D.hpp"
#include "include/FSM3D.hpp"
#include "include/random_source3D.hpp"
#include "include/euclidien3D.hpp"
#include "include/write_result3D.hpp"
#include "include/write_result2D.hpp"


/**
 * @brief dans ce main on compare le temps d'execution des 3 méthodes en fonctions du nombre de points en fonction d'un nombre de source
 */
int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << "<n_source> <num_n> <n> <n1> <n2>  ... <nn> " << std::endl;
        return 1;
    }

    int n_source = std::stoi(argv[1]);
    int nb_n = std::stoi(argv[2]);
    if (argc !=  3 +   nb_n) {
        std::cerr << "Usage: " << argv[0] << " <n_source> <num_n> <n> <n1> <n2>  ... <nn> " << std::endl;
        return 1;
    }

    std::vector<std::pair<int, int>> Xs;

    std::ostringstream filename;
    filename << "/ccc/work/cont001/ocre/demutha/results/result_comparaison"  << ".txt";
    std::ofstream file(filename.str(), std::ios::out);
    if (!file) {
        std::cerr << "Erreur lors de l'ouverture du fichier!" << std::endl;
        return 1;
    }

    double length = 10;
    double t = 0;
    int n_tab[nb_n];
    for(int i = 0 ; i < nb_n ; i++){
        n_tab[i] = std::stoi(argv[3+i]);
    }

    
    std::vector<double> time_fmm;
    std::vector<double> time_fim;
    std::vector<double> time_fsm;
    std::vector<double> time_exact;

    int n;
    for (int i = 0; i < nb_n; i++) {
        
        
        n = n_tab[i];
        std::cout << "n_source " << n_source << " n points = " << n << std::endl; 
        std::vector<std::vector<std::vector<double>>> F(n, std::vector<std::vector<double>>(n, std::vector<double>(n, 1.0)));
    std::vector<std::vector<std::vector<double>>> T_FIM(n, std::vector<std::vector<double>>(n, std::vector<double>(n, std::numeric_limits<double>::infinity())));
    std::vector<std::vector<std::vector<double>>> closest_FIM(n, std::vector<std::vector<double>>(n, std::vector<double>(n, 0)));
    std::vector<std::vector<std::vector<double>>> T_FSM(n, std::vector<std::vector<double>>(n, std::vector<double>(n, std::numeric_limits<double>::infinity())));
    std::vector<std::vector<std::vector<double>>> lock_FSM(n, std::vector<std::vector<double>>(n, std::vector<double>(n, 0)));
    std::vector<std::vector<std::vector<double>>> closest_FSM(n, std::vector<std::vector<double>>(n, std::vector<double>(n, 0)));
    std::vector<std::vector<std::vector<double>>> T_exact(n, std::vector<std::vector<double>>(n, std::vector<double>(n, std::numeric_limits<double>::infinity())));
    std::vector<std::vector<std::vector<double>>> closest_exact(n, std::vector<std::vector<double>>(n, std::vector<double>(n, 0)));

    std::vector<std::tuple<int,int,int>> Xs;
    
    generate_source(Xs,n,n_source,12345,true);
        //t = FMM(T_FMM, n, length, Xs, closest_FMM);
        time_fmm.push_back(0);
        t = FSM3D(T_FSM,n,length,Xs,closest_FSM,lock_FSM);
        time_fsm.push_back(t);
        t = FIM3D(T_FIM, n, length, Xs, closest_FIM);
        time_fim.push_back(t);
        t = euclidien3d(T_exact, Xs, n, n, length, closest_exact);
        time_exact.push_back(t);
        std::cout << n << " " << n << std::endl;

        
    }
    saveResults(file, n_source, nb_n, n_tab, time_fmm, time_fsm, time_fim, time_exact);
    file.close();

    return 0;
}
