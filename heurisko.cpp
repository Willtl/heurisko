
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
#include "solvers/parameters.h"
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
    const int runningTime = 14400;
    const std::vector<std::vector<double>> distMatrix = reader::readTspInstance(reader::TspInstance::A280);
    const std::shared_ptr<TravellingSalesmanProblem> tsp = make_shared<TravellingSalesmanProblem>(distMatrix.size(), distMatrix, OptimizationStrategy::MINIMIZE, RepresentationType::INDIRECT);

    // Check solvers/parameter.h file to understand the definition of the parameters
    DifferentialEvolutionParameters parameters = {8, 0.0, 0.5, std::log10(tsp->getDimension()), false};
    DifferentialEvolution<encoding> de(parameters, tsp);
    de.setRunningTime(runningTime);
    de.solve();
}

// Example using Differential Evolution to solve the Rastrigin function
void continuousOptimizationProblemExample() {
    const int dimension = 100;
    const int runningTime = 600;
    const std::shared_ptr<RastriginFunction> rastrigin = make_shared<RastriginFunction>(dimension, OptimizationStrategy::MINIMIZE, RepresentationType::INDIRECT);

    DifferentialEvolutionParameters parameters = {8, 0.5 / dimension, 0.9, runningTime + 1, false};
    DifferentialEvolution<encoding> de(parameters, rastrigin);
    de.setRunningTime(runningTime);
    de.solve();
}
