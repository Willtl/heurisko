#ifndef ACKLEY_H
#define ACKLEY_H

#include "../../../entities/problem.h"

class AckleyFunction : public Problem<double>
{
public:
    AckleyFunction(OptimizationStrategy strategy, RepresentationType repType) : Problem(strategy, repType)
    {
        this->lb = vector<double>{-5, -5};
        this->ub = vector<double>{5, 5};
        this->dimension = 2;
    }

    double evaluate(const vector<double> &decisionVariables) override
    {
        const double x = decisionVariables[0];
        const double y = decisionVariables[1];
        return -20 * exp(-0.2 * sqrt(0.5 * (pow(x, 2) + pow(y, 2)))) - exp(0.5 * (cos(2 * M_PI * x) + cos(2 * M_PI * y))) + exp(1) + 20;
    }
};

#endif // ACKLEY_H
