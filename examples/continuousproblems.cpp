
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
void continuousDirectExample();
void continuousIndirectExample();

int main(int argc, char *argv[])
{
    continuousDirectExample();
    continuousIndirectExample();
    exit(EXIT_SUCCESS);
}

void continuousDirectExample()
{
    const std::shared_ptr<AckleyFunction> prob = std::make_shared<AckleyFunction>();

    DifferentialEvolutionParameters parameters = {8, 0.2, 0.9, 600 + 1, false};
    DifferentialEvolution<encoding> de(parameters, prob);
    de.setRunningTime(600);
    de.solve();
}

void continuousIndirectExample()
{
    const int dimension = 100;
    const std::shared_ptr<RastriginFunction> prob = std::make_shared<RastriginFunction>(dimension);

    DifferentialEvolutionParameters parameters = {8, 0.5 / dimension, 0.9, 600 + 1, false};
    DifferentialEvolution<encoding> de(parameters, prob);
    de.setRunningTime(600);
    de.solve();
}
