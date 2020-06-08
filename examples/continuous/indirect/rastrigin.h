#ifndef RASTRIGIN_H
#define RASTRIGIN_H

#include "../../../entities/problem.h"

typedef double encoding;

class RastriginSolution : public Solution<encoding>
{
public:
    RastriginSolution(int dimension, std::vector<encoding> &decVar) : Solution(decVar)
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
        this->lb = std::vector<encoding>(dimension);
        this->ub = std::vector<encoding>(dimension);
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
            sum += std::pow(decisionVariables[i], 2) - A * cos(2 * M_PI * decisionVariables[i]);
        }
        return A * dimension + sum;
    }

    std::shared_ptr<Solution<double>> construct(std::vector<encoding> &decisionVariables) override
    {
        std::shared_ptr<RastriginSolution> solution(new RastriginSolution(this->dimension, decisionVariables));
        return solution;
    }
};

#endif // RASTRIGIN_H
