#ifndef DIFFERENTIALEVOLUTION_H
#define DIFFERENTIALEVOLUTION_H

#include "../entities/problem.h"
#include "../entities/solution.h"
#include "globalsolver.h"
#include <iostream>
#include <memory>

template <class T> class DifferentialEvolution : public GlobalSolver<T> {
    public:
    DifferentialEvolution(int numberOfIndividuals, float recombinationRate, float differentialWeight, std::shared_ptr<Problem<T>> prob) : GlobalSolver<T>(numberOfIndividuals, prob) {
        if (this->numberOfAgents < 4) {
            std::cerr << "The number of individuals needs to be equal or higher than 4" << std::endl;
            exit(EXIT_FAILURE);
        }

        this->recombinationRate = recombinationRate;
        this->differentialWeight = differentialWeight;
        std::puts("DifferentialEvolution instantiated");
    }

    void solve() {
        if (this->maxIterations == 0 && this->runningTime == 0) {
            std::cerr << "Use \"setMaxIterations(int)\" or \"setRunningTime(double)\" to "
                     "define a stopping criteria!"
                  << std::endl;
            exit(EXIT_FAILURE);
        } else
            std::cout << "Starting DifferentialEvolution search procedure" << std::endl;

        utils::startTimeCounter();

        // Current population
        std::cout << this->numberOfAgents << std::endl;
        std::vector<std::vector<double>> individuals(this->numberOfAgents);
        //#pragma omp parallel for
        for (size_t i = 0; i < this->numberOfAgents; i++)
            this->problem->fillRandomDecisionVariables(individuals[i]);

        // Stores the objective value of each individual
        std::vector<double> individualsFitness(this->numberOfAgents);
#pragma omp parallel for
        for (int i = 0; i < this->numberOfAgents; i++) {
            switch (this->problem->getRepType()) {
            case RepresentationType::DIRECT:
                individualsFitness[i] = this->problem->evaluate(individuals[i]);
                break;
            case RepresentationType::INDIRECT:
                std::shared_ptr<Solution<T>> sol = this->problem->construct(individuals[i]);
                individualsFitness[i] = sol->getFitness();
                break;
            }
#pragma omp critical
            this->updateGlobalBest(individuals[i], individualsFitness[i], true);
        }

        // Store the new individuals (new position) after the crossover
        std::vector<std::vector<double>> newIndividuals = individuals;
        // Stores the objective value of each new individual
        std::vector<double> newIndividualsFitness = individualsFitness;

        int iteration = -1;
        while (iteration++ < this->maxIterations || utils::getCurrentTime() < this->runningTime) {
#pragma omp parallel for
            for (int i = 0; i < this->numberOfAgents; i++) {
                // Pick 3 different agents
                int a = utils::getRandom(this->numberOfAgents - 1);
                int b = utils::getRandom(this->numberOfAgents - 1);
                int c = utils::getRandom(this->numberOfAgents - 1);
                while (a == i)
                    a = utils::getRandom(this->numberOfAgents - 1);
                while (a == b || b == i)
                    b = utils::getRandom(this->numberOfAgents - 1);
                while (c == a || c == b || c == i)
                    c = utils::getRandom(this->numberOfAgents - 1);

                int R = utils::getRandom(this->problem->getDimension() - 1);
                for (size_t j = 0; j < this->problem->getDimension(); j++) {
                    // newIndividuals[x][i] =
                    // this->prob->getFeasibleDecisionVariable(i);

                    if (utils::getRandom() < recombinationRate || j == R) {
                        newIndividuals[i][j] = std::max(
                            this->problem->getLb()[j], std::min(individuals[a][j] + (differentialWeight * (individuals[b][j] - individuals[c][j])), this->problem->getUb()[j]));
                    } else
                        newIndividuals[i][j] = individuals[i][j];
                }
            }

            switch (this->problem->getStrategy()) {
            case OptimizationStrategy::MINIMIZE:
#pragma omp parallel for
                for (int i = 0; i < this->numberOfAgents; i++) {
                    switch (this->problem->getRepType()) {
                    case RepresentationType::DIRECT:
                        newIndividualsFitness[i] = this->problem->evaluate(newIndividuals[i]);
                        if (newIndividualsFitness[i] <= individualsFitness[i]) {
                            individuals[i] = newIndividuals[i];
                            individualsFitness[i] = newIndividualsFitness[i];
#pragma omp critical
                            this->updateGlobalBest(individuals[i], individualsFitness[i], true);
                        }
                        break;
                    case RepresentationType::INDIRECT:
                        std::shared_ptr<Solution<T>> sol = this->problem->construct(newIndividuals[i]);
                        newIndividualsFitness[i] = sol->getFitness();
                        if (newIndividualsFitness[i] <= individualsFitness[i]) {
                            individuals[i] = newIndividuals[i];
                            individualsFitness[i] = newIndividualsFitness[i];
#pragma omp critical
                            this->updateGlobalBest(individuals[i], individualsFitness[i], true, sol);
                        }
                        break;
                    }
                }
                break;
            case OptimizationStrategy::MAXIMIZE:
#pragma omp parallel for
                for (int i = 0; i < this->numberOfAgents; i++) {
                    newIndividualsFitness[i] = this->problem->evaluate(newIndividuals[i]);
                    if (newIndividualsFitness[i] >= individualsFitness[i]) {
                        individuals[i] = newIndividuals[i];
                        individualsFitness[i] = newIndividualsFitness[i];
#pragma omp critical
                        this->updateGlobalBest(individuals[i], individualsFitness[i], true);
                    }
                }
                break;
            }
        }

        switch (this->problem->getRepType()) {
        case RepresentationType::DIRECT:
            std::cout << "Best solution " << this->globalBestFitness << " Running time: " << utils::getCurrentTime() << std::endl << "Best decision variables: ";
            utils::printVector(this->globalBest);
            std::cout << "Number of tried solutions " << this->problem->getNumbTriedSolution() << std::endl;
            break;
        case RepresentationType::INDIRECT:
            std::cout << "Best solution " << this->globalBestFitness << " Running time: " << utils::getCurrentTime() << std::endl;
            std::cout << "Best decision variables: ";
            utils::printVector(this->globalBest);
            std::cout << "Best solution: ";
            this->bestSolution->print();
            std::cout << "Number of tried solutions " << this->problem->getNumbTriedSolution() << std::endl;
            break;
        }
    }

    private:
    float recombinationRate;
    float differentialWeight;
};

#endif // DIFFERENTIALEVOLUTION_H
