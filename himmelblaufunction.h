#ifndef HIMMELBLAUFUNCTION_H
#define HIMMELBLAUFUNCTION_H

#include "math.h"
#include "problem.h"

using namespace std;

class HimmelblauFunction : public Problem<double>
{
public:
    HimmelblauFunction(OptimizationStrategy strategy) : Problem(strategy)
    {
        this->lb = vector<double>{-5, -5};
        this->ub = vector<double>{5, 5};
        this->dimension = 2;
    }

    double evaluate(const vector<double> &decisionVariables) override
    {
        const double x = decisionVariables[0];
        const double y = decisionVariables[1];

        return pow(pow(x, 2) + y - 11, 2) + pow(x + pow(y, 2) - 7, 2);
    }
};

#endif // HIMMELBLAUFUNCTION_H
