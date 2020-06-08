#ifndef RASTRIGINFUNCTION_H
#define RASTRIGINFUNCTION_H

#include "math.h"
#include "problem.h"

using namespace std;

class RastriginFunction : public Problem<double>
{
public:
    RastriginFunction(int dimension, OptimizationStrategy strategy) : Problem(strategy)
    {
        this->lb = vector<double>(dimension);
        this->ub = vector<double>(dimension);
        this->dimension = dimension;

        for (size_t i = 0; i < this->dimension; i++) {
            lb[i] = -5.12;
            ub[i] = 5.12;
        }
    }

    double evaluate(const vector<double> &decisionVariables) override
    {
        double A = 10, sum = 0;
        for (size_t i = 0; i < this->dimension; i++) {
            sum += pow(decisionVariables[i], 2) - A * cos(2 * M_PI * decisionVariables[i]);
        }
        return A * dimension + sum;
    }
};

#endif // RASTRIGINFUNCTION_H
