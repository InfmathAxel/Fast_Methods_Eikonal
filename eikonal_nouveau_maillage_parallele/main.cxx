
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <limits>
#include "include/FIM.hpp"
#include "include/FSM.hpp"
#include "include/FMM.hpp"
#include "include/random_source.hpp"
#include "include/write_result.hpp"
#include "include/euclidien.hpp"
#include "include/source.hpp"
#include "include/maille.hpp"


/**
 * Dans ce main on teste les méthodes en séquentielle sur des mailles plutots qu'avec les indices de matrices, en particulier les sources sont générées aléatoirement 
 * dans la maille, et le reste des points au centre de la maille pour évité les equidistances.
 */
int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <n> <n_source>" << std::endl;
        return 1;
    }

    int n = std::stoi(argv[1]);
    int n_source = std::stoi(argv[2]);
    double length = 10;
    double steps = length / (n - 1);

    std::vector<std::vector<double>> T_FMM(n, std::vector<double>(n, std::numeric_limits<double>::infinity()));
    std::vector<std::vector<double>> closest_FMM(n, std::vector<double>(n, 0));
    std::vector<std::vector<double>> T_FIM(n, std::vector<double>(n, std::numeric_limits<double>::infinity()));
    std::vector<std::vector<double>> closest_FIM(n, std::vector<double>(n, 0));
    std::vector<std::vector<double>> T_exact(n, std::vector<double>(n, std::numeric_limits<double>::infinity()));
    std::vector<std::vector<double>> closest_FSM(n, std::vector<double>(n, 0));
    std::vector<std::vector<double>> T_FSM(n, std::vector<double>(n, std::numeric_limits<double>::infinity()));

    std::vector<source> Xs;
    meshes mesh(n, n, length, steps);

    generate_source(Xs, mesh, n, n_source, steps);
    for (auto &Xi : Xs) {
        Xi.print();
    }

    //FMM(T_FMM, n, length, Xs, closest_FMM, mesh);
    FIM(T_FIM, n, length, Xs, closest_FIM, mesh);
    FSM(T_FSM, n, length, Xs, closest_FSM, mesh);
    euclidien(T_exact, Xs, n, n, length);

    std::string tag_fmm = "FMM";
    std::string tag_fim = "FIM";
    std::string tag_fsm = "FSM";

    write_files(T_FMM, length, n, n_source, tag_fmm);
    write_closest(closest_FMM, Xs, length, n, n_source, tag_fmm);
    write_files(T_FIM, length, n, n_source, tag_fim);
    write_closest(closest_FIM, Xs, length, n, n_source, tag_fim);
    write_files(T_FSM, length, n, n_source, tag_fsm);
    write_closest(closest_FSM, Xs, length, n, n_source, tag_fsm);
    write_files(T_exact, length, n, n_source, "exact");
    save(Xs, "coords_mesh.txt");

    return 0;
}