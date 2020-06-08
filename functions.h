#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "math.h"
#include "problem.h"
using namespace std;

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

/*class BoothFunction : public Problem<double>
{
public:
    BoothFunction(OptimizationStrategy strategy) : Problem(strategy)
    {
        this->lb = vector<double>{-10, -10};
        this->ub = vector<double>{10, 10};
        this->dimension = 2;
    }

    double evaluate( vector<double> &decisionVariables) override
    {
        const double x = decisionVariables[0];
        const double y = decisionVariables[1];
        return pow(x + 2 * y - 7, 2) + pow(2 * x + y - 5, 2);
    }
};

class CrossInTrayFunction : public Problem<double>
{
public:
    CrossInTrayFunction(OptimizationStrategy strategy) : Problem(strategy)
    {
        this->lb = vector<double>{-10, -10};
        this->ub = vector<double>{10, 10};
        this->dimension = 2;
    }

    double evaluate( vector<double> &decisionVariables) override
    {
        const double x = decisionVariables[0];
        const double y = decisionVariables[1];

        double absolute1 = abs(100 - (sqrt(pow(x, 2) + pow(y, 2)) / M_PI));
        double absolute2 = abs(sin(x) * sin(y) * exp(absolute1)) + 1;
        return -0.0001 * pow(absolute2, 0.1);
    }
};

class HimmelblauFunction : public Problem<double>
{
public:
    HimmelblauFunction(OptimizationStrategy strategy) : Problem(strategy)
    {
        this->lb = vector<double>{-5, -5};
        this->ub = vector<double>{5, 5};
        this->dimension = 2;
    }

    double evaluate( vector<double> &decisionVariables) override
    {
        const double x = decisionVariables[0];
        const double y = decisionVariables[1];

        return pow(pow(x, 2) + y - 11, 2) + pow(x + pow(y, 2) - 7, 2);
    }
};

class SphereFunction : public Problem<double>
{
public:
    SphereFunction(OptimizationStrategy strategy) : Problem(strategy)
    {
        this->lb = vector<double>{-DBL_MAX, -DBL_MAX};
        this->ub = vector<double>{DBL_MAX, DBL_MAX};
        this->dimension = 2;
    }

    double evaluate( vector<double> &decisionVariables) override
    {
        const double x = decisionVariables[0];
        const double y = decisionVariables[1];
        return pow(x, 2) + pow(y, 2);
    }
};*/

/*class RastriginFunction : public Problem<double>
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

    double evaluate( vector<double> &decisionVariables) override
    {
        double A = 10, sum = 0;
        for (size_t i = 0; i < this->dimension; i++) {
            sum += pow(decisionVariables[i], 2) - A * cos(2 * M_PI * decisionVariables[i]);
        }
        return A * dimension + sum;
    }
};*/

#endif // FUNCTIONS_H
