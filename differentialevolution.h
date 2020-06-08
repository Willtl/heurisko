#ifndef DIFFERENTIALEVOLUTION_H
#define DIFFERENTIALEVOLUTION_H

#include "globalsolver.h"
#include "problem.h"
#include "solution.h"
#include <iostream>
#include <memory>

using namespace std;

template <class T>
class DifferentialEvolution : public GlobalSolver<T>
{
public:
    DifferentialEvolution(int numberOfIndividuals, float recombinationRate, float differentialWeight, shared_ptr<Problem<T>> prob)
        : GlobalSolver<T>(numberOfIndividuals, prob)
    {
        if (this->numberOfAgents < 4) {
            cerr << "The number of individuals needs to be equal or higher than 4" << endl;
            exit(EXIT_FAILURE);
        }

        this->recombinationRate = recombinationRate;
        this->differentialWeight = differentialWeight;
        puts("DifferentialEvolution instantiated");
    }

    void solve()
    {
        if (this->maxIterations == 0 && this->runningTime == 0) {
            cerr << "Use \"setMaxIterations(int)\" or \"setRunningTime(double)\" to define a stopping criteria!" << endl;
            exit(EXIT_FAILURE);
        } else
            cout << "Starting DifferentialEvolution search procedure" << endl;

        utils::startTimeCounter();

        // Current population
        cout << this->numberOfAgents << endl;
        vector<vector<double>> individuals(this->numberOfAgents);
        //#pragma omp parallel for
        for (size_t i = 0; i < this->numberOfAgents; i++)
            this->problem->fillRandomDecisionVariables(individuals[i]);

        // Stores the objective value of each individual
        vector<double> individualsFitness(this->numberOfAgents);
        //#pragma omp parallel for
        for (size_t i = 0; i < this->numberOfAgents; i++) {
            individualsFitness[i] = this->problem->evaluate(individuals[i]);
#pragma omp critical
            this->updateGlobalBest(individuals[i], individualsFitness[i], true);
        }

        // Store the new individuals (new position) after the crossover
        vector<vector<double>> newIndividuals = individuals;
        // Stores the objective value of each new individual
        vector<double> newIndividualsFitness = individualsFitness;

        int iteration = -1;
        while (iteration++ < this->maxIterations || utils::getCurrentTime() < this->runningTime) {
#pragma omp parallel for
            for (size_t i = 0; i < this->numberOfAgents; i++) {
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
                    // newIndividuals[x][i] = this->prob->getFeasibleDecisionVariable(i);

                    if (utils::getRandom() < recombinationRate || j == R) {
                        newIndividuals[i][j] =
                            max(this->problem->getLb()[j],
                            min(individuals[a][j] + (differentialWeight * (individuals[b][j] - individuals[c][j])), this->problem->getUb()[j]));
                    } else
                        newIndividuals[i][j] = individuals[i][j];
                }
            }

            switch (this->problem->getStrategy()) {
            case OptimizationStrategy::MINIMIZE:
                //#pragma omp parallel for
                for (size_t i = 0; i < this->numberOfAgents; i++) {
                    switch (this->problem->getRepType()) {
                    case RepresentationType::DIRECT:
                        newIndividualsFitness[i] = this->problem->evaluate(newIndividuals[i]);
                        break;
                    case RepresentationType::INDIRECT:
                        /*for (size_t l1 = 0; l1 < this->problem->getDimension(); l1++) {
                            cout << newIndividuals[i][l1] << ", ";
                        }
                        cout << endl;*/

                        shared_ptr<Solution<T>> sol = this->problem->construct(newIndividuals[i]);
                        newIndividualsFitness[i] = sol->getFitness();

                        // this->problem->localSearch(sol);

                        /*for (size_t l1 = 0; l1 < this->problem->getDimension(); l1++) {
                            cout << newIndividuals[i][l1] << ", ";
                        }
                        cout << endl;
                        exit(0);*/
                    }

                    if (newIndividualsFitness[i] <= individualsFitness[i]) {
                        individuals[i] = newIndividuals[i];
                        individualsFitness[i] = newIndividualsFitness[i];
#pragma omp critical
                        this->updateGlobalBest(individuals[i], individualsFitness[i], true);
                    }
                }
                break;
            case OptimizationStrategy::MAXIMIZE:
                //#pragma omp parallel for
                for (size_t i = 0; i < this->numberOfAgents; i++) {
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
        cout << "Best solution " << this->globalBestFitness << " Running time: " << utils::getCurrentTime() << endl << "Best solution decision variables: ";
        utils::printVector(this->globalBest);

        cout << "Number of tried solutions " << this->problem->getNumbTriedSolution() << endl;
    }

private:
    float recombinationRate;
    float differentialWeight;
};

#endif // DIFFERENTIALEVOLUTION_H
