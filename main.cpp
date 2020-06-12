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
    std::vector<int> optRoute = {1,  22, 8,  26, 31, 28, 3,  36, 35, 20, 2,  29, 21, 16, 50, 34, 30, 9,  49, 10, 39, 33, 45, 15, 44, 42,
                     40, 19, 41, 13, 25, 14, 24, 43, 7,  23, 48, 6,  27, 51, 46, 12, 47, 18, 4,  17, 37, 5,  38, 11, 32};
    for (size_t i = 0; i < dimension; i++)
        optRoute[i] = optRoute[i] - 1;
    std::vector<double> decisionVariables(dimension);
    tsp::encode(optRoute, decisionVariables);
    std::shared_ptr<TSPSolution> solution = std::make_shared<TSPSolution>(dimension, decisionVariables);
    std::cout << "Fitness: " << solution->getFitness() << "\n";
    utils::printVector<std::vector<int>>(solution->getPermutation());

    // Create random solution for debugging
    /*std::vector<double> decisionVariables(dimension);
    tsp->fillRandomDecisionVariables(decisionVariables);
    std::shared_ptr<TSPSolution> solution = std::make_shared<TSPSolution>(dimension, decisionVariables);
    utils::printVector<std::vector<double>>(decisionVariables);
    utils::printVector<std::vector<int>>(solution->getPermutation());
    std::cout << "Fitness: " << solution->getFitness() << "\n";
    solution->print();
    solution->localSearch();
    utils::printVector<std::vector<double>>(solution->getDecisionVariables());
    utils::printVector<std::vector<int>>(solution->getPermutation());
    std::cout << "Fitness: " << solution->getFitness() << "\n";*/
}

int main(int argc, char *argv[])
{
    std::vector<std::pair<int, int>> rawNodes = reader::readTSPInstance("/home/willian/Gitkraken/heurisko/examples/instances/tsp/eil51.tsp");
    // std::vector<std::pair<int, int>> rawNodes = reader::readTSPInstance("E:/Users/main/Documents/GitHub/heurisko/examples/instances/tsp/eil51.tsp");
    std::shared_ptr<TravellingSalesmanProblem> tsp =
        make_shared<TravellingSalesmanProblem>(rawNodes.size(), rawNodes, OptimizationStrategy::MINIMIZE, RepresentationType::INDIRECT);

    testWithSingleSolution(rawNodes.size(), tsp);
    exit(0);

    DifferentialEvolution<encoding> de(8, 1.0 / tsp->getDimension(), 1.1, tsp, 5);
    de.setRunningTime(300);
    de.solve();

    /*GeneticAlgorithm<double> ga(8, 1.0, 1.0 / tsp->getDimension(), CrossoverType::UNIFORM, SelectionType::TOURNAMENT, MutationType::RANDOM_MUTATION, tsp);
    ga.setRunningTime(25.0);
    ga.solve();*/

    /*GreyWolfOptimizer<double> gwo(8, tsp);
    gwo.setRunningTime(60.0);
    gwo.solve();*/

    /*ParticleSwarmOptimization<double> pso(32, 2.1, 2.1, 0.4, 0.9, 1.0, tsp);
    pso.setRunningTime(20.0);
    pso.solve();*/

    exit(EXIT_SUCCESS);
}
