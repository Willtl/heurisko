#include <QCoreApplication>

#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include "differentialevolution.h"
#include "functions.h"
#include "geneticalgorithm.h"
#include "greywolfoptimizer.h"
#include "particleswarmoptimization.h"
#include "solution.h"
#include "util.h"

using namespace std;
typedef double encoding;

class RastriginSolution : public Solution<encoding>
{
public:
    RastriginSolution(int dimension, vector<encoding> &decVar) : Solution(decVar)
    {
        this->dimension = dimension;

        /*if(utils::getRandom() <= 0.0001)
            this->localSearch();*/

        double A = 10, sum = 0;
        for (size_t i = 0; i < dimension; i++) {
            sum += pow(decisionVariables[i], 2) - A * cos(2 * M_PI * decisionVariables[i]);
        }
        this->fitness = A * dimension + sum;
    }

    void localSearch() override
    {
        for (size_t i = 0; i < dimension; i++)
            this->decisionVariables[i] = 0;
    }

protected:
    int dimension;
};

class RastriginFunction : public Problem<encoding>
{
public:
    RastriginFunction(int dimension, OptimizationStrategy strategy, RepresentationType repType) : Problem(strategy, repType)
    {
        this->lb = vector<encoding>(dimension);
        this->ub = vector<encoding>(dimension);
        this->dimension = dimension;

        for (size_t i = 0; i < this->dimension; i++) {
            lb[i] = -5.12;
            ub[i] = 5.12;
        }
    }

    double evaluate(const vector<encoding> &decisionVariables) override
    {
        double A = 10, sum = 0;
        for (size_t i = 0; i < this->dimension; i++) {
            sum += pow(decisionVariables[i], 2) - A * cos(2 * M_PI * decisionVariables[i]);
        }
        return A * dimension + sum;
    }

    shared_ptr<Solution<double>> construct(vector<encoding> &decisionVariables) override
    {
        shared_ptr<RastriginSolution> solution(new RastriginSolution(this->dimension, decisionVariables));
        return solution;
    }
};

int main(int argc, char *argv[])
{
    shared_ptr<RastriginFunction> problem(new RastriginFunction(100, OptimizationStrategy::MINIMIZE, RepresentationType::INDIRECT));

    DifferentialEvolution<encoding> de(32, 0.05, 0.9, problem);
    de.setRunningTime(60.0);
    de.solve();

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
