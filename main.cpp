#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include "entities/solution.h"
#include "examples/combinatorial/indirect/tsp.h"
#include "examples/continuous/direct/ackley.h"
#include "examples/continuous/direct/crossintray.h"
#include "examples/continuous/direct/functions.h"
#include "examples/continuous/indirect/rastrigin.h"
#include "solvers/differentialevolution.h"
#include "solvers/geneticalgorithm.h"
#include "solvers/greywolfoptimizer.h"
#include "solvers/particleswarmoptimization.h"
#include "util/util.h"

int main(int argc, char *argv[]) {
    std::vector<double> xnodes = {0, 1, 2, 3, 4, 5, 6, 7, 8, 7, 5};
    std::vector<double> ynodes = {0, 2, 3, 3, 4, 5, 4, 3, 2, 1, 0};
    std::shared_ptr<TravellingSalesmanProblem> tsp(new TravellingSalesmanProblem(xnodes.size(), xnodes, ynodes, OptimizationStrategy::MINIMIZE, RepresentationType::INDIRECT));

    /*DifferentialEvolution<encoding> de(32, 0.001, 0.5, tsp);
    de.setRunningTime(3);
    de.solve();*/

    GeneticAlgorithm<double> ga(32, 0.8, 0.01, CrossoverType::UNIFORM, SelectionType::TOURNAMENT, MutationType::POLYNOMIAL, tsp);
    ga.setRunningTime(600.0);
    ga.setEtaC(2.0);
    ga.setEtaM(20.0);
    ga.solve();

    /*std::vector<double> decisionVariables(4);
    tsp->fillRandomDecisionVariables(decisionVariables);
    utils::printVector<vector<double>>(decisionVariables);
    std::shared_ptr<Solution<double>> sol = tsp->construct(decisionVariables);
    std::cout << sol->getFitness() << std::endl;
    sol->print();*/

    /*shared_ptr<RastriginFunction> problem(new RastriginFunction(100,
    OptimizationStrategy::MINIMIZE, RepresentationType::INDIRECT));
    DifferentialEvolution<encoding> de(32, 0.001, 0.5, problem);
    de.setRunningTime(60.0);
    de.solve();*/

    /*GeneticAlgorithm<double> ga(128, 0.8, 0.01, CrossoverType::UNIFORM,
    SelectionType::TOURNAMENT, MutationType::POLYNOMIAL, problem);
    ga.setRunningTime(600.0);
    ga.setEtaC(2.0);
    ga.setEtaM(20.0);
    ga.solve();*/

    //    GreyWolfOptimizer<double> gwo(32, problem);
    //    gwo.setRunningTime(60.0);
    //    gwo.solve();

    /*ParticleSwarmOptimization<double> pso(32, 2.05, 2.05, 0.4, 0.9, problem);
    pso.setRunningTime(120.0);
    pso.solve();*/

    exit(EXIT_SUCCESS);
}
