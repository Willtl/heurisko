#ifndef BOOTHFUNCTION_H
#define BOOTHFUNCTION_H

#include "math.h"
#include "problem.h"

using namespace std;

class BoothFunction : public Problem<double>
{
public:
    BoothFunction(OptimizationStrategy strategy) : Problem(strategy)
    {
        this->lb = vector<double>{-10, -10};
        this->ub = vector<double>{10, 10};
        this->dimension = 2;
    }

    double evaluate(const vector<double> &decisionVariables) override
    {
        const double x = decisionVariables[0];
        const double y = decisionVariables[1];
        return pow(x + 2 * y - 7, 2) + pow(2 * x + y - 5, 2);
    }
};

#endif // BOOTHFUNCTION_H
