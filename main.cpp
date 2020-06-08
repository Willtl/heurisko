#include <QCoreApplication>

#include <chrono>
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

#include "examples/continuous/direct/ackley.h"
#include "examples/continuous/direct/crossintray.h"
#include "examples/continuous/direct/functions.h"
#include "examples/continuous/indirect/rastrigin.h"

using namespace std;

int main(int argc, char *argv[])
{
    shared_ptr<CrossInTrayFunction> problem(new CrossInTrayFunction(OptimizationStrategy::MINIMIZE, RepresentationType::DIRECT));
    DifferentialEvolution<encoding> de(32, 0.05, 0.9, problem);
    de.setRunningTime(60.0);
    de.solve();

    /*shared_ptr<RastriginFunction> problem(new RastriginFunction(100, OptimizationStrategy::MINIMIZE, RepresentationType::INDIRECT));

    DifferentialEvolution<encoding> de(32, 0.05, 0.9, problem);
    de.setRunningTime(60.0);
    de.solve();*/

    // shared_ptr<AckleyFunction> problem(new AckleyFunction(OptimizationStrategy::Minimize));
    // shared_ptr<BoothFunction> problem(new BoothFunction(OptimizationStrategy::Minimize));
    // shared_ptr<SphereFunction> problem(new SphereFunction(OptimizationStrategy::Minimize));
    // shared_ptr<CrossInTrayFunction> problem(new CrossInTrayFunction(OptimizationStrategy::Minimize));
    // shared_ptr<HimmelblauFunction> problem(new HimmelblauFunction(OptimizationStrategy::Minimize));

    /*GeneticAlgorithm<double> ga(128, 0.99, 0.01, CrossoverType::SIMULATED_BINARY, SelectionType::TOURNAMENT, MutationType::POLYNOMIAL, problem);
    ga.setRunningTime(600.0);
    ga.setEtaC(2.0);
    ga.setEtaM(90.0);
    ga.solve();*/

    /*GreyWolfOptimizer<double> gwo(128, problem);
    gwo.setRunningTime(5.0);
    gwo.solve();*/

    /*ParticleSwarmOptimization<double> pso(32, 2.05, 2.05, 0.4, 0.9, problem);
    pso.setRunningTime(120.0);
    pso.solve();*/

    exit(EXIT_SUCCESS);
}
