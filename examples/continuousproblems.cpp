
#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include "entities/solution.h"
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

// Continuous problem with indirect representation
void continuousIndirectExample();
void continuousDirectExample();

int main(int argc, char *argv[])
{
    continuousDirectExample();
    continuousIndirectExample();

    exit(EXIT_SUCCESS);
}

void continuousIndirectExample()
{
    const int dimension = 100;
    const std::shared_ptr<RastriginFunction> rastrigin = make_shared<RastriginFunction>(dimension, OptimizationStrategy::MINIMIZE, RepresentationType::INDIRECT);

    DifferentialEvolutionParameters parameters = {8, 0.5 / dimension, 0.9, 600 + 1, false};
    DifferentialEvolution<encoding> de(parameters, rastrigin);
    de.setRunningTime(600);
    de.solve();
}

void continuousDirectExample()
{
    const std::shared_ptr<AckleyFunction> rastrigin = make_shared<AckleyFunction>(OptimizationStrategy::MINIMIZE, RepresentationType::DIRECT);

    DifferentialEvolutionParameters parameters = {8, 0.2, 0.9, 600 + 1, false};
    DifferentialEvolution<encoding> de(parameters, rastrigin);
    de.setRunningTime(600);
    de.solve();
}
