#ifndef GLOBALSOLVER_H
#define GLOBALSOLVER_H

#include "solver.h"
#include <memory>
#include <vector>

template <class T>
class GlobalSolver : public Solver<T>
{
public:
    GlobalSolver(int numberOfAgents, std::shared_ptr<Problem<T>> prob) : Solver<T>(prob)
    {
        if (numberOfAgents % 2 != 0)
            numberOfAgents += 1;

        this->numberOfAgents = numberOfAgents;

        switch (prob->getStrategy()) {
        case OptimizationStrategy::MINIMIZE:
            this->globalBestFitness = DBL_MAX;
            break;
        case OptimizationStrategy::MAXIMIZE:
            this->globalBestFitness = -DBL_MAX;
            std::cout << "global fitness initialized " << globalBestFitness << std::endl;
            break;
        }

        puts("Global solver instantiated");
    }


protected:
    int numberOfAgents;
    std::vector<double> globalBest;
    double globalBestFitness;

    bool updateGlobalBest(const std::vector<double> individual, double fitness, bool printUpdate)
    {
        switch (this->problem->getStrategy()) {
        case OptimizationStrategy::MINIMIZE:
            if (fitness < globalBestFitness) {
                globalBest = individual;
                globalBestFitness = fitness;
                if (printUpdate)
                    utils::printValueAndTime(fitness, utils::getCurrentTime());
                return true;
            }
            break;
        case OptimizationStrategy::MAXIMIZE:
            if (fitness > globalBestFitness) {
                globalBest = individual;
                globalBestFitness = fitness;
                if (printUpdate)
                    utils::printValueAndTime(fitness, utils::getCurrentTime());
                return true;
            }
            break;
        }
        return false;
    }
};

#endif // GLOBALSOLVER_H
