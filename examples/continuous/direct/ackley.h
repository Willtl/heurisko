#ifndef ACKLEY_H
#define ACKLEY_H

#include "../../../src/problem.h"

class AckleyFunction : public Problem<double>
{
public:
    // Minimize ackley function (two dimensional) with direct representation
    AckleyFunction() : Problem(2, OptimizationStrategy::MINIMIZE, RepresentationType::DIRECT)
    {
        this->lb = std::vector<double>{-5, -5};
        this->ub = std::vector<double>{5, 5};
    }

    double evaluate(const std::vector<double> &decisionVariables) override
    {
        const double x = decisionVariables[0];
        const double y = decisionVariables[1];
        return -20 * exp(-0.2 * sqrt(0.5 * (pow(x, 2) + pow(y, 2)))) - exp(0.5 * (cos(2 * M_PI * x) + cos(2 * M_PI * y))) + exp(1) + 20;
    }
};

#endif // ACKLEY_H
