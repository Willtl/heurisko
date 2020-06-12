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
#include "solvers/iteratedlocalseach.h"
#include "solvers/particleswarmoptimization.h"
#include "util/tspreader.h"
#include "util/util.h"

void testWithSingleSolution(int dimension, std::shared_ptr<TravellingSalesmanProblem> tsp)
{
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
    std::vector<std::pair<double, double>> rawNodes = reader::readTSPInstance("/home/willian/Gitkraken/heurisko/examples/instances/tsp/eil76.tsp");
    // std::vector<std::pair<double, double>> rawNodes = reader::readTSPInstance("E:/Users/main/Documents/GitHub/heurisko/examples/instances/tsp/eil51.tsp");
    std::shared_ptr<TravellingSalesmanProblem> tsp =
        make_shared<TravellingSalesmanProblem>(rawNodes.size(), rawNodes, OptimizationStrategy::MINIMIZE, RepresentationType::INDIRECT);

    IteratedLocalSearch<encoding> ils(8, 1, tsp);
    ils.setRunningTime(60);
    ils.solve();

    /*DifferentialEvolution<encoding> de(8, 0.0, 0.5, tsp, 0);
    de.setRunningTime(60);
    de.solve();*/

    /*GeneticAlgorithm<double> ga(8, 1.0, 1.0 / tsp->getDimension(), CrossoverType::UNIFORM, SelectionType::TOURNAMENT, MutationType::RANDOM_MUTATION, tsp);
    ga.setRunningTime(60);
    ga.solve();*/

    /*GreyWolfOptimizer<double> gwo(8, tsp);
    gwo.setRunningTime(60.0);
    gwo.solve();*/

    /*ParticleSwarmOptimization<double> pso(32, 2.1, 2.1, 0.4, 0.9, 1.0, tsp);
    pso.setRunningTime(20.0);
    pso.solve();*/

    exit(EXIT_SUCCESS);
}
