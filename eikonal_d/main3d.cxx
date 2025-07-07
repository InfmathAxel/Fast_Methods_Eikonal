#include "include/FIM3D.hpp"
#include "include/FSM3D.hpp"
#include "include/random_source3D.hpp"
#include "include/euclidien3D.hpp"
#include "include/write_result3D.hpp"

double error(const std::vector<std::vector<std::vector<double>>> &euc, const std::vector<std::vector<std::vector<double>>> &T,int n){
    double mean = 0 ;
    
    for (int i = 0 ; i < n; i++   ){
        for (int j = 0 ; j < n ; j++){
            for(int k = 0 ; k < n ; k++){
            mean += std::abs(euc[i][j][k]-T[i][j][k]);
            
        }
    }
    }
    return mean/(n*n*n);
}


int main(int argc, char* argv[]){


    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <n> <length> <n_source>" << std::endl;
        return 1;
    }

    int n = std::stoi(argv[1]);
    double length = std::stod(argv[2]);
    int n_source = std::stoi(argv[3]);
    
    double inf = std::numeric_limits<double>::infinity();
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
    
    FIM3D(T_FIM,n,length,Xs,closest_FIM);
    FSM3D(T_FSM,n,length,Xs,closest_FSM,lock_FSM);
    euclidien3d(T_exact,Xs,n,n,length,closest_exact);
    writeVTKFile("FSM_vtk",T_FSM,length/(n-1));
    writeMatrixToFile(T_FIM,"results/FIM_result.txt") ;
    writeMatrixToFile(T_FSM,"results/FSM_result.txt") ;
    writeMatrixToFile(T_exact,"results/exact_result.txt") ;

    std::cout << "erreur  moyenne fim : " << error(T_exact,T_FIM,n) << "\n erreur moyenne FSM "<< error(T_exact,T_FSM,n) << std::endl;
    return 0;
}