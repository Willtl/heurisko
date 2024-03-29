#include <iostream>
#include <memory>
#include <random>
#include <vector>
#include <thread>

#include "../src/solution.h"
#include "combinatorial/indirect/tsp.h"
#include "../src/solvers/differentialevolution.h"
#include "../src/solvers/iteratedlocalseach.h"
#include "../src/solvers/parameters.h"
#include "../src/util/timer.h"
#include "../src/util/tspreader.h"
#include "../src/util/util.h"

void travellingSalesmanProblemExample();

int main(int argc, char *argv[])
{
    travellingSalesmanProblemExample();
    exit(EXIT_SUCCESS);
}

void travellingSalesmanProblemExample()
{
    // Load distance matrix from instance file
    const std::vector<std::vector<int>> distMatrix = reader::readTspInstance(reader::TspInstance::VLSI_XQL662);

    // Problem object used to generate solutions of the problem and perform local search
    const std::shared_ptr<TravellingSalesmanProblem> tsp =
        std::make_shared<TravellingSalesmanProblem>(distMatrix.size(), distMatrix, OptimizationStrategy::MINIMIZE, RepresentationType::INDIRECT);

    // Usually setting the number of individuals to be the same as the number of CPU threads is a good idea
    unsigned int num_individual = std::thread::hardware_concurrency();

    // Check solvers/parameter.h file to understand the definition of the parameters
    DifferentialEvolutionParameters parameters = {num_individual, 0.0, 1.1, std::log10(tsp->getDimension()) + 5, false};

    // Solver
    DifferentialEvolution<encoding> de(parameters, tsp);
    de.setRunningTime(600);
    de.solve();

    /* IteratedLocalSearch<encoding> it(num_individual, 1, tsp);
    it.setRunningTime(600);
    it.solve(); */
}
