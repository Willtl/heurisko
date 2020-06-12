#ifndef SOLVER_H
#define SOLVER_H

#include "../entities/problem.h"
#include "../entities/solution.h"
#include <memory>

template <class T>
class Solver
{
public:
    enum StoppingCriteria { Iteration, Time };
    void setMaxIterations(int maxNumberOfIterations) { maxIterations = maxNumberOfIterations; }
    void setRunningTime(double seconds)
    {
        utils::maxRunningTime = seconds;
        runningTime = seconds;
    }
    virtual void solve() = 0;

protected:
    Solver(std::shared_ptr<Problem<T>> prob)
    {
        this->problem = prob;
        switch (this->problem->getStrategy()) {
        case OptimizationStrategy::MINIMIZE:
            std::cout << "Instantiating Solver for Minimization" << std::endl;
            break;
        case OptimizationStrategy::MAXIMIZE:
            std::cout << "Instantiating  Solver for Maximization" << std::endl;
            break;
        }
    }

    std::shared_ptr<Problem<T>> problem;
    int maxIterations = 0;
    double runningTime = 0;
};

#endif // SOLVER_H
