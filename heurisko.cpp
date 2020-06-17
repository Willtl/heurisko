
#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include "entities/solution.h"
#include "examples/combinatorial/indirect/tsp.h"
#include "examples/combinatorial/indirect/vrp.h"
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
#include "util/timer.h"
#include "util/tspreader.h"
#include "util/util.h"

void createSolution();
void travellingSalesmanProblemExample();
void continuousOptimizationProblemExample();
void vehicleRoutingProblemExample();

int main(int argc, char *argv[])
{
    // travellingSalesmanProblemExample();
    // continuousOptimizationProblemExample();
    vehicleRoutingProblemExample();
    /*{
        Timer timer;
        createSolution();
    }*/
    exit(EXIT_SUCCESS);
}

void createSolution()
{
    std::vector<std::vector<double>> distanceMatrix = {
        {0, 548, 776, 696, 582, 274, 502, 194, 308, 194, 536, 502, 388, 354, 468, 776, 662},
        {548, 0, 684, 308, 194, 502, 730, 354, 696, 742, 1084, 594, 480, 674, 1016, 868, 1210},
        {776, 684, 0, 992, 878, 502, 274, 810, 468, 742, 400, 1278, 1164, 1130, 788, 1552, 754},
        {696, 308, 992, 0, 114, 650, 878, 502, 844, 890, 1232, 514, 628, 822, 1164, 560, 1358},
        {582, 194, 878, 114, 0, 536, 764, 388, 730, 776, 1118, 400, 514, 708, 1050, 674, 1244},
        {274, 502, 502, 650, 536, 0, 228, 308, 194, 240, 582, 776, 662, 628, 514, 1050, 708},
        {502, 730, 274, 878, 764, 228, 0, 536, 194, 468, 354, 1004, 890, 856, 514, 1278, 480},
        {194, 354, 810, 502, 388, 308, 536, 0, 342, 388, 730, 468, 354, 320, 662, 742, 856},
        {308, 696, 468, 844, 730, 194, 194, 342, 0, 274, 388, 810, 696, 662, 320, 1084, 514},
        {194, 742, 742, 890, 776, 240, 468, 388, 274, 0, 342, 536, 422, 388, 274, 810, 468},
        {536, 1084, 400, 1232, 1118, 582, 354, 730, 388, 342, 0, 878, 764, 730, 388, 1152, 354},
        {502, 594, 1278, 514, 400, 776, 1004, 468, 810, 536, 878, 0, 114, 308, 650, 274, 844},
        {388, 480, 1164, 628, 514, 662, 890, 354, 696, 422, 764, 114, 0, 194, 536, 388, 730},
        {354, 674, 1130, 822, 708, 628, 856, 320, 662, 388, 730, 308, 194, 0, 342, 422, 536},
        {468, 1016, 788, 1164, 1050, 514, 514, 662, 320, 274, 388, 650, 536, 342, 0, 764, 194},
        {776, 868, 1552, 560, 674, 1050, 1278, 742, 1084, 810, 1152, 274, 388, 422, 764, 0, 798},
        {662, 1210, 754, 1358, 1244, 708, 480, 856, 514, 468, 354, 844, 730, 536, 194, 798, 0},
    };
    const std::shared_ptr<VehicleRoutingProblem> tsp =
        make_shared<VehicleRoutingProblem>(distanceMatrix.size(), 0, 4, distanceMatrix, OptimizationStrategy::MINIMIZE, RepresentationType::INDIRECT);
    std::vector<std::vector<double>> decisionVariables(10000000, std::vector<double>(distanceMatrix.size() * 2));
#pragma omp parallel for
    for (int i = 0; i < 10000000; i++) {
        tsp->fillRandomDecisionVariables(decisionVariables[i]);
        tsp->construct(decisionVariables[i]);
    }
}

// Example using Iterated Local Search or Differential Evolution to solve the travelling salesman problem
void travellingSalesmanProblemExample()
{
    const std::vector<std::vector<double>> distMatrix = reader::readTspInstance(reader::TspInstance::A280);
    const std::shared_ptr<TravellingSalesmanProblem> tsp =
        make_shared<TravellingSalesmanProblem>(distMatrix.size(), distMatrix, OptimizationStrategy::MINIMIZE, RepresentationType::INDIRECT);

    // Check solvers/parameter.h file to understand the definition of the parameters
    DifferentialEvolutionParameters parameters = {8, 0.0, 0.5, std::log10(tsp->getDimension()), false};
    DifferentialEvolution<encoding> de(parameters, tsp);
    de.setRunningTime(600);
    de.solve();
}

// Example using Differential Evolution to solve the Rastrigin function
void continuousOptimizationProblemExample()
{
    const int dimension = 100;
    const std::shared_ptr<RastriginFunction> rastrigin = make_shared<RastriginFunction>(dimension, OptimizationStrategy::MINIMIZE, RepresentationType::INDIRECT);

    DifferentialEvolutionParameters parameters = {8, 0.5 / dimension, 0.9, 600 + 1, false};
    DifferentialEvolution<encoding> de(parameters, rastrigin);
    de.setRunningTime(600);
    de.solve();
}

void vehicleRoutingProblemExample()
{
    std::vector<std::vector<double>> distanceMatrix = {
        {0, 548, 776, 696, 582, 274, 502, 194, 308, 194, 536, 502, 388, 354, 468, 776, 662},
        {548, 0, 684, 308, 194, 502, 730, 354, 696, 742, 1084, 594, 480, 674, 1016, 868, 1210},
        {776, 684, 0, 992, 878, 502, 274, 810, 468, 742, 400, 1278, 1164, 1130, 788, 1552, 754},
        {696, 308, 992, 0, 114, 650, 878, 502, 844, 890, 1232, 514, 628, 822, 1164, 560, 1358},
        {582, 194, 878, 114, 0, 536, 764, 388, 730, 776, 1118, 400, 514, 708, 1050, 674, 1244},
        {274, 502, 502, 650, 536, 0, 228, 308, 194, 240, 582, 776, 662, 628, 514, 1050, 708},
        {502, 730, 274, 878, 764, 228, 0, 536, 194, 468, 354, 1004, 890, 856, 514, 1278, 480},
        {194, 354, 810, 502, 388, 308, 536, 0, 342, 388, 730, 468, 354, 320, 662, 742, 856},
        {308, 696, 468, 844, 730, 194, 194, 342, 0, 274, 388, 810, 696, 662, 320, 1084, 514},
        {194, 742, 742, 890, 776, 240, 468, 388, 274, 0, 342, 536, 422, 388, 274, 810, 468},
        {536, 1084, 400, 1232, 1118, 582, 354, 730, 388, 342, 0, 878, 764, 730, 388, 1152, 354},
        {502, 594, 1278, 514, 400, 776, 1004, 468, 810, 536, 878, 0, 114, 308, 650, 274, 844},
        {388, 480, 1164, 628, 514, 662, 890, 354, 696, 422, 764, 114, 0, 194, 536, 388, 730},
        {354, 674, 1130, 822, 708, 628, 856, 320, 662, 388, 730, 308, 194, 0, 342, 422, 536},
        {468, 1016, 788, 1164, 1050, 514, 514, 662, 320, 274, 388, 650, 536, 342, 0, 764, 194},
        {776, 868, 1552, 560, 674, 1050, 1278, 742, 1084, 810, 1152, 274, 388, 422, 764, 0, 798},
        {662, 1210, 754, 1358, 1244, 708, 480, 856, 514, 468, 354, 844, 730, 536, 194, 798, 0},
    };
    const std::shared_ptr<VehicleRoutingProblem> vrp =
        make_shared<VehicleRoutingProblem>(distanceMatrix.size(), 0, 4, distanceMatrix, OptimizationStrategy::MINIMIZE, RepresentationType::INDIRECT);

    // Check solvers/parameter.h file to understand the definition of the parameters
    DifferentialEvolutionParameters parameters = {8, 0.0, 0.9, 5 + 1, false};
    DifferentialEvolution<encoding> de(parameters, vrp);
    de.setRunningTime(5);
    de.solve();
}
