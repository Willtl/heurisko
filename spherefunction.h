#ifndef SPHEREFUNCTION_H
#define SPHEREFUNCTION_H

#include "math.h"
#include "problem.h"

using namespace std;

class SphereFunction : public Problem<double>
{
public:
    SphereFunction(OptimizationStrategy strategy) : Problem(strategy)
    {
        this->lb = vector<double>{-DBL_MAX, -DBL_MAX};
        this->ub = vector<double>{DBL_MAX, DBL_MAX};
        this->dimension = 2;
    }

    double evaluate(const vector<double> &decisionVariables) override
    {
        const double x = decisionVariables[0];
        const double y = decisionVariables[1];
        return pow(x, 2) + pow(y, 2);
    }
};

#endif // SPHEREFUNCTION_H
