#ifndef CROSSINTRAY_H
#define CROSSINTRAY_H

#include "../../../src/problem.h"

class CrossInTrayFunction : public Problem<double>
{
public:
    CrossInTrayFunction(OptimizationStrategy strategy, RepresentationType repType) : Problem(2, strategy, repType)
    {
        this->lb = std::vector<double>{-10, -10};
        this->ub = std::vector<double>{10, 10};
    }

    double evaluate(const std::vector<double> &decisionVariables) override
    {
        const double x = decisionVariables[0];
        const double y = decisionVariables[1];

        double absolute1 = abs(100 - (sqrt(pow(x, 2) + pow(y, 2)) / M_PI));
        double absolute2 = abs(sin(x) * sin(y) * exp(absolute1)) + 1;
        return -0.0001 * pow(absolute2, 0.1);
    }
};

#endif // CROSSINTRAY_H
