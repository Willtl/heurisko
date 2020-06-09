#include <cmath>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include "entities/solution.h"
#include "solvers/differentialevolution.h"
#include "solvers/geneticalgorithm.h"
#include "solvers/greywolfoptimizer.h"
#include "solvers/particleswarmoptimization.h"
#include "util/util.h"

#include "examples/combinatorial/indirect/tsp.h"
#include "examples/continuous/direct/ackley.h"
#include "examples/continuous/direct/crossintray.h"
#include "examples/continuous/direct/functions.h"
#include "examples/continuous/indirect/rastrigin.h"

using namespace std;

int main(int argc, char *argv[])
{
    //    shared_ptr<RastriginFunction> problem(new RastriginFunction(1000, OptimizationStrategy::MINIMIZE, RepresentationType::INDIRECT));

    vector<double> xnodes = {-7, 17, 2, 2};
    vector<double> ynodes = {-4, 6.5, 1, 2};
    shared_ptr<TSP> tsp(new TSP(4, xnodes, ynodes, OptimizationStrategy::MINIMIZE, RepresentationType::INDIRECT));

    exit(0);
    DifferentialEvolution<encoding> de(32, 0.0, 0.5, tsp);
    de.setRunningTime(60.0);
    de.solve();

    /*GeneticAlgorithm<double> ga(128, 0.8, 0.01, CrossoverType::UNIFORM, SelectionType::TOURNAMENT, MutationType::POLYNOMIAL, problem);
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
