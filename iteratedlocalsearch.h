#ifndef ITERATEDLOCALSEARCH_H
#define ITERATEDLOCALSEARCH_H

#include "../entities/problem.h"
#include "../entities/solution.h"
#include "globalsolver.h"
#include <iostream>
#include <memory>

template <class T>
class IteratedLocalSearch : public GlobalSolver<T>
{
public:
        IteratedLocalSearch(int numberOfSolutions, int perturbationStrength, std::shared_ptr<Problem<T>> prob) : GlobalSolver<T>(numberOfSolutions, prob)
        {
                this->perturbationStrength = perturbationStrength;
                this->numberOfAgents = 8; // numb of threads
                if (this->perturbationStrength < 1) {
                        std::cerr << "The perturbation rate must be higher than one" << std::endl;
                        exit(EXIT_FAILURE);
                }

                std::puts("IteratedLocalSearch instantiated");
        }

        void perturbate(std::vector<T> &decisionVariables)
        {
                for (size_t i = 0; i < perturbationStrength; i++) {
                        int index = utils::getRandom(this->problem->getDimension() - 1);
                        decisionVariables[index] = this->problem->getRandomDecisionVariableAt(index);
                }
        }

        void swapPerturbate(std::vector<T> &decisionVariables)
        {
                for (size_t i = 0; i < perturbationStrength; i++) {
                        int index1 = utils::getRandom(this->problem->getDimension() - 1);
                        int index2 = utils::getRandom(this->problem->getDimension() - 1);
                        std::swap(decisionVariables[index1], decisionVariables[index2]);
                }
        }

        void solve()
        {
                if (this->maxIterations == 0 && this->runningTime == 0) {
                        std::cerr << "Use \"setMaxIterations(int)\" or \"setRunningTime(double)\" to "
                                     "define a stopping criteria!"
                                  << std::endl;
                        exit(EXIT_FAILURE);
                } else
                        std::cout << "Starting IteratedLocalSearch search procedure" << std::endl;

                utils::startTimeCounter();

                // Current population
                std::cout << this->numberOfAgents << std::endl;
                std::vector<std::vector<double>> solutions(this->numberOfAgents);
#pragma omp parallel for
                for (size_t i = 0; i < this->numberOfAgents; i++)
                        this->problem->fillRandomDecisionVariables(solutions[i]);

                // Stores the objective value of each individual
                std::vector<double> solutionsFitness(this->numberOfAgents);
#pragma omp parallel for
                for (int i = 0; i < this->numberOfAgents; i++) {
                        switch (this->problem->getRepType()) {
                        case RepresentationType::DIRECT:
                                solutionsFitness[i] = this->problem->evaluate(solutions[i]);
                                break;
                        case RepresentationType::INDIRECT:
                                std::shared_ptr<Solution<T>> sol = this->problem->construct(solutions[i]);
                                solutionsFitness[i] = sol->getFitness();
                                break;
                        }
#pragma omp critical
                        this->updateGlobalBest(solutions[i], solutionsFitness[i], true);
                }

                int iteration = -1;
                while (iteration++ < this->maxIterations || utils::getCurrentTime() < this->runningTime) {

#pragma omp parallel for
                        for (int i = 0; i < this->numberOfAgents; i++) {
                                solutions[i] = std::vector<T>(this->globalBest);
                                swapPerturbate(solutions[i]);
                        }

                        switch (this->problem->getStrategy()) {
                        case OptimizationStrategy::MINIMIZE:
#pragma omp parallel for
                                for (int i = 0; i < this->numberOfAgents; i++) {
                                        switch (this->problem->getRepType()) {
                                        case RepresentationType::DIRECT:
                                                solutionsFitness[i] = this->problem->evaluate(solutions[i]);
                                                this->updateGlobalBest(solutions[i], solutionsFitness[i], true);
                                                break;
                                        case RepresentationType::INDIRECT:
                                                std::shared_ptr<Solution<T>> sol = this->problem->construct(solutions[i]);
                                                sol->localSearch();
                                                solutionsFitness[i] = sol->getFitness();
                                                this->updateGlobalBest(solutions[i], solutionsFitness[i], true, sol);
                                                break;
                                        }
                                }
                                break;
                        case OptimizationStrategy::MAXIMIZE:

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
        int perturbationStrength;
};

#endif // ITERATEDLOCALSEARCH_H
