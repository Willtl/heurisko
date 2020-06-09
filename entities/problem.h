#ifndef PROBLEM_H
#define PROBLEM_H

#include "../entities/solution.h"
#include "../util/util.h"
#include <vector>

enum OptimizationStrategy { MINIMIZE, MAXIMIZE };
enum RepresentationType { DIRECT, INDIRECT };

template <class T>
class Problem
{
public:
    int getDimension() const { return dimension; }
    std::vector<T> getLb() const { return lb; }
    std::vector<T> getUb() const { return ub; }
    OptimizationStrategy getStrategy() const { return strategy; }
    std::type_info getProblemType() const { return typeid(T); }
    int getNumbTriedSolution() const { return numbTriedSolution; }
    RepresentationType getRepType() const { return repType; }
    void setRepType(const RepresentationType &value) { repType = value; }

    virtual double evaluate(const std::vector<T> &decisionVariables)
    {
        std::cerr << "You must override @evaluate in your derived class" << std::endl;
        exit(EXIT_FAILURE);
    }

    virtual std::shared_ptr<Solution<T>> construct(std::vector<T> &decisionVariables)
    {
        std::cerr << "You must override @construct in your derived class" << std::endl;
        exit(EXIT_FAILURE);
    }

    virtual void localSearch(std::shared_ptr<Solution<T>> &solution) {}

    virtual void fillRandomDecisionVariables(std::vector<T> &variables)
    {
        if (typeid(T) == typeid(double)) {
            variables = std::vector<T>(dimension);
            for (size_t i = 0; i < dimension; i++) {
                variables[i] = getRandomDecisionVariableAt(i);
            }
        } else {
            puts("You must override the fillRandomDecisionVariable in the derived class");
            exit(EXIT_FAILURE);
        }
    }

    virtual T getRandomDecisionVariableAt(int indexVar)
    {
        if (typeid(T) == typeid(double)) {
            return lb[indexVar] + utils::getRandom() * (ub[indexVar] - lb[indexVar]);
        } else {
            puts("You must override the getRandomDecisionVariableAt in the derived class");
            exit(EXIT_FAILURE);
        }
    }

protected:
    Problem(OptimizationStrategy strategy, RepresentationType repType)
    {
        this->strategy = strategy;
        this->repType = repType;
    }

    OptimizationStrategy strategy;
    RepresentationType repType;
    std::vector<T> lb;
    std::vector<T> ub;
    int dimension;
    int numbTriedSolution;
    double current;
    double max;
};

#endif // PROBLEM_H
