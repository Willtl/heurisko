
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

void travellingSalesmanProblemExample();
void continuousOptimizationProblemExample();

int main(int argc, char *argv[]) {
    travellingSalesmanProblemExample();
    // continuousOptimizationProblemExample();
    exit(EXIT_SUCCESS);
}

// Example using Iterated Local Search or Differential Evolution to solve the travelling salesman problem
void travellingSalesmanProblemExample() {
    const int runningTime = 600;
    const std::vector<std::pair<double, double>> rawNodes = reader::readTspInstance(reader::TspInstance::A280);
    const std::shared_ptr<TravellingSalesmanProblem> tsp = make_shared<TravellingSalesmanProblem>(rawNodes.size(), rawNodes, OptimizationStrategy::MINIMIZE, RepresentationType::INDIRECT);

    /*const int numbIndividuals = 8;
    const float crossoverRate = 0.0; // Seems like that for high-dimensional problems, 0.0 leads to better performance
    const float differentialWeight = 0.5;
    const double localSearchAfterTime = std::log10(tsp->getDimension()); // how much time the solver must wait for no improvement to start using local search
    DifferentialEvolution<encoding> de(numbIndividuals, crossoverRate, differentialWeight, tsp, localSearchAfterTime);
    de.setRunningTime(runningTime);
    de.solve();*/

    const int numbSolutions = 8; // number of solutions per iteration, it turns out that it is good to keep 1 solution per core
    const int perturbationStrength = 1;
    IteratedLocalSearch<encoding> ils(numbSolutions, perturbationStrength, tsp);
    ils.setRunningTime(60);
    ils.solve();
}

// Example using Differential Evolution to solve the Rastrigin function
void continuousOptimizationProblemExample() {
    const int dimension = 500;
    const int runningTime = 600;
    const std::shared_ptr<RastriginFunction> tsp = make_shared<RastriginFunction>(dimension, OptimizationStrategy::MINIMIZE, RepresentationType::INDIRECT);

    const int numberOfIndividuals = 16;
    const float crossoverRate = 0.5 / dimension;
    const float differentialWeight = 0.9;
    const double localSearchAfterTime = runningTime + 1;

    DifferentialEvolution<encoding> de(numberOfIndividuals, crossoverRate, differentialWeight, tsp, localSearchAfterTime);
    de.setRunningTime(runningTime);
    de.solve();

    /*GeneticAlgorithm<double> ga(8, 1.0, 1.0 / tsp->getDimension(), CrossoverType::UNIFORM, SelectionType::TOURNAMENT, MutationType::RANDOM_MUTATION, tsp);
    ga.setRunningTime(60);
    ga.solve();*/
}
