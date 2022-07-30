
#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include "../src/solution.h"
#include "continuous/direct/ackley.h"
#include "continuous/direct/crossintray.h"
#include "continuous/direct/functions.h"
#include "continuous/indirect/rastrigin.h"
#include "../src/solvers/differentialevolution.h"
#include "../src/solvers/geneticalgorithm.h"
#include "../src/solvers/greywolfoptimizer.h"
#include "../src/solvers/iteratedlocalseach.h"
#include "../src/solvers/parameters.h"
#include "../src/solvers/particleswarmoptimization.h"
#include "../src/util/timer.h" 
#include "../src/util/util.h"

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
