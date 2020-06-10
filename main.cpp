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
#include "util/tspreader.h"
#include "util/util.h"

void testWithSingleSolution(int dimension, std::shared_ptr<TravellingSalesmanProblem> tsp)
{
    // Create random solution for debugging
    std::vector<double> decisionVariables(dimension);
    tsp->fillRandomDecisionVariables(decisionVariables);
    std::shared_ptr<TSPSolution> solution(new TSPSolution(dimension, decisionVariables));
    utils::printVector<std::vector<double>>(decisionVariables);
    utils::printVector<std::vector<int>>(solution->getPermutation());
    std::cout << "Fitness: " << solution->getFitness() << "\n";
    solution->localSearch();
    utils::printVector<std::vector<double>>(decisionVariables);
    utils::printVector<std::vector<int>>(solution->getPermutation());
    std::cout << "Fitness: " << solution->getFitness() << "\n";
}

int main(int argc, char *argv[])
{
    std::vector<std::pair<int, int>> rawNodes = reader::readTSPInstance("/home/willian/Gitkraken/heurisko/examples/instances/tsp/eil51.tsp");
    // std::vector<std::pair<int, int>> rawNodes = reader::readTSPInstance("E:/Users/main/Documents/GitHub/heurisko/examples/instances/tsp/eil51.tsp");
    std::shared_ptr<TravellingSalesmanProblem> tsp(new TravellingSalesmanProblem(rawNodes.size(), rawNodes, OptimizationStrategy::MINIMIZE, RepresentationType::INDIRECT));

    /*testWithSingleSolution(rawNodes.size(), tsp);
    exit(0); */
    DifferentialEvolution<encoding> de(8, 0.005, 0.25, tsp);
    de.setRunningTime(300);
    de.solve();

    /*GeneticAlgorithm<double> ga(32, 0.8, 0.01, CrossoverType::UNIFORM, SelectionType::TOURNAMENT, MutationType::POLYNOMIAL, tsp);
    ga.setRunningTime(600.0);
    ga.setEtaC(2.0);
    ga.setEtaM(20.0);
    ga.solve();*/

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

    /*GreyWolfOptimizer<double> gwo(32, problem);
    gwo.setRunningTime(60.0);
    gwo.solve();*/

    /*ParticleSwarmOptimization<double> pso(32, 2.05, 2.05, 0.4, 0.9, problem);
    pso.setRunningTime(120.0);
    pso.solve();*/

    exit(EXIT_SUCCESS);
}
