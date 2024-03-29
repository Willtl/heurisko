#ifndef RASTRIGIN_H
#define RASTRIGIN_H

#include "../../../src/problem.h"

typedef double encoding;

class RastriginSolution : public Solution<encoding>
{
public:
    RastriginSolution(const int dimension, std::vector<encoding> &decVar) : Solution(dimension, decVar)
    {
        double A = 10, sum = 0;
        for (size_t i = 0; i < dimension; i++) {
            sum += pow(decisionVariables[i], 2) - A * cos(2 * M_PI * decisionVariables[i]);
        }
        this->fitness = A * dimension + sum;
    }

protected:
    int dimension;
};

class RastriginFunction : public Problem<encoding>
{
public:
    RastriginFunction(const int dimension) : Problem(dimension, OptimizationStrategy::MINIMIZE, RepresentationType::INDIRECT)
    {
        this->lb = std::vector<encoding>(dimension);
        this->ub = std::vector<encoding>(dimension);

        for (size_t i = 0; i < this->dimension; i++) {
            lb[i] = -5.12;
            ub[i] = 5.12;
        }
    }

    std::shared_ptr<Solution<double>> construct(std::vector<encoding> &decisionVariables) override
    {
        std::shared_ptr<RastriginSolution> solution = std::make_shared<RastriginSolution>(this->dimension, decisionVariables);
        return solution;
    }
};

#endif // RASTRIGIN_H
