#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H

#include "solver.h"
#include "solution.h"

#include <memory>
#include <vector>

template <class T1, class T2>
class LocalSearch : public Solver<T1>
{
public:
    LocalSearch(std::shared_ptr<Solution<T2>> solution, std::shared_ptr<Problem<T1>> prob) : Solver<T1>(prob) {
        this->initialSolution = solution;
    }
protected:
    std::shared_ptr<Solution<T2>> initialSolution;
    void start();
};

#endif // LOCALSEARCH_H
