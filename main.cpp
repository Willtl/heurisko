
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

void travellingSalesmanProblemExample() {
    const std::vector<std::pair<double, double>> rawNodes = reader::readTspInstance(reader::TspInstance::A280);
    const std::shared_ptr<TravellingSalesmanProblem> tsp = make_shared<TravellingSalesmanProblem>(rawNodes.size(), rawNodes, OptimizationStrategy::MINIMIZE, RepresentationType::INDIRECT);

    const int numberOfIndividuals = 8;
    const float crossoverRate = 0.0; // 1.0 / tsp->getDimension();
    const float differentialWeight = 0.5;
    const double localSearchAfterTime = std::log10(tsp->getDimension()); // how much time the solver must wait for no improvement to start using local search
    DifferentialEvolution<encoding> de(numberOfIndividuals, crossoverRate, differentialWeight, tsp, localSearchAfterTime);
    de.setRunningTime(600);
    de.solve();
}

int main(int argc, char *argv[]) {
    travellingSalesmanProblemExample();

    /*IteratedLocalSearch<encoding> ils(8, 1, tsp);
    ils.setRunningTime(60);
    ils.solve();*/

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
