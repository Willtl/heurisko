#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include "entities/solution.h"
#include "examples/combinatorial/indirect/tsp.h"
#include "solvers/differentialevolution.h"
#include "solvers/parameters.h"
#include "util/timer.h"
#include "util/tspreader.h"
#include "util/util.h"

void travellingSalesmanProblemExample();

int main(int argc, char *argv[])
{
    travellingSalesmanProblemExample();
    exit(EXIT_SUCCESS);
}

void travellingSalesmanProblemExample()
{
    // Load distance matrix from instance file
    const std::vector<std::vector<double>> distMatrix = reader::readTspInstance(reader::TspInstance::A280);
    // Problem object used to generate solutions of the problem and perform local search
    const std::shared_ptr<TravellingSalesmanProblem> tsp =
        std::make_shared<TravellingSalesmanProblem>(distMatrix.size(), distMatrix, OptimizationStrategy::MINIMIZE, RepresentationType::INDIRECT);

    // Check solvers/parameter.h file to understand the definition of the parameters
    DifferentialEvolutionParameters parameters = {8, 0.0, 0.5, std::log10(tsp->getDimension()), false};

    // Solver
    DifferentialEvolution<encoding> de(parameters, tsp);
    de.setRunningTime(600);
    de.solve();
}
