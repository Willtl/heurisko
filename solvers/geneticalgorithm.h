#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include "../entities/problem.h"
#include "globalsolver.h"
#include <iostream>
#include <memory>

enum CrossoverType { UNIFORM, ONE_POINT, TWO_POINT, SIMULATED_BINARY };
enum SelectionType { FITNESS_PROPORTIONATE, TOURNAMENT };
enum MutationType { RANDOM_MUTATION, POLYNOMIAL, SWAP_MUTATION, SCRAMBLE_MUTATION };

template <class T>
class GeneticAlgorithm : public GlobalSolver<T>
{
public:
    GeneticAlgorithm(int numberOfIndividuals, float crossoverRate, float mutationRate, CrossoverType crossoverType, SelectionType selectionType, MutationType mutationType,
             std::shared_ptr<Problem<T>> prob)
        : GlobalSolver<T>(numberOfIndividuals, prob)
    {
        if (this->numberOfAgents < 2) {
            std::cerr << "The number of individuals needs to be equal or higher than 2" << std::endl;
            exit(EXIT_FAILURE);
        }

        this->crossoverRate = crossoverRate;
        this->mutationRate = mutationRate;
        this->crossoverType = crossoverType;
        this->selectionType = selectionType;
        this->mutationType = mutationType;
        puts("Genetic Algorithm instantiated");
    }

    void solve()
    {
        if (this->maxIterations == 0 && this->runningTime == 0) {
            std::cerr << "Use \"setMaxIterations(int)\" or \"setRunningTime(double)\" to "
                     "define a stopping criteria!"
                  << std::endl;
            exit(EXIT_FAILURE);
        } else
            std::cout << "Starting Genetic Algorithm search procedure" << std::endl;

        if (this->mutationType == MutationType::POLYNOMIAL && this->etaM == -FLT_MIN) {
            std::cerr << "You must set the polynomial mutation index through "
                     "\"setEtaM(float)\" function"
                  << std::endl;
            exit(EXIT_FAILURE);
        } else if (this->crossoverType == CrossoverType::SIMULATED_BINARY && this->etaC == -FLT_MIN) {
            std::cerr << "You must set the SBX operator index through \"setEtaC(float)\" "
                     "function"
                  << std::endl;
            exit(EXIT_FAILURE);
        }

        utils::startTimeCounter();

        // Current population
        std::vector<std::vector<double>> individuals(this->numberOfAgents);
#pragma omp parallel for
        for (int i = 0; i < this->numberOfAgents; i++)
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

        // Container to store the offspring after the crossover (child1 at newIndividuals1
        // and child2 at newIndividuals2)
        std::vector<std::vector<double>> newIndividuals1 = individuals;
        std::vector<std::vector<double>> newIndividuals2 = individuals;
        // Objective values for the offspring
        std::vector<double> newIndividuals1Fitness = individualsFitness;
        std::vector<double> newIndividuals2Fitness = individualsFitness;

        int iteration = -1;
        while (iteration++ < this->maxIterations || utils::getCurrentTime() < this->runningTime) {
#pragma omp parallel for
            for (int i = 0; i < this->numberOfAgents; i++) {
                // Select two individuals
                int indexParent1, indexParent2;
                selection(indexParent1, indexParent2, individualsFitness);
                if (utils::getRandom() <= this->crossoverRate) {
                    // Perform crossover
                    crossover(individuals[indexParent1], individuals[indexParent2], newIndividuals1[i], newIndividuals2[i]);
                    // Mutate the new individuals
                    mutate(newIndividuals1[i], newIndividuals2[i]);
                } else {
                    newIndividuals1[i] = individuals[indexParent1];
                    newIndividuals2[i] = individuals[indexParent2];
                }
            }

#pragma omp parallel for
            for (int i = 0; i < this->numberOfAgents; i++) {
                // Evaluate child1
                switch (this->problem->getRepType()) {
                case RepresentationType::DIRECT:
                    newIndividuals1Fitness[i] = this->problem->evaluate(newIndividuals1[i]);
                    newIndividuals2Fitness[i] = this->problem->evaluate(newIndividuals2[i]);
                    break;
                case RepresentationType::INDIRECT:
                    std::shared_ptr<Solution<T>> sol1 = this->problem->construct(newIndividuals1[i]);
                    newIndividuals1Fitness[i] = sol1->getFitness();
                    std::shared_ptr<Solution<T>> sol2 = this->problem->construct(newIndividuals2[i]);
                    newIndividuals2Fitness[i] = sol2->getFitness();

                    if (utils::getCurrentTime() > 30) {
                        sol1->localSearch();
                        sol2->localSearch();
                    }
                    break;
                }
#pragma omp critical
                {
                    this->updateGlobalBest(newIndividuals1[i], newIndividuals1Fitness[i], true);
                    this->updateGlobalBest(newIndividuals2[i], newIndividuals2Fitness[i], true);
                }
            }

            // Pick the best individuals in the offspring to be the next generation
            switch (this->problem->getStrategy()) {
            case OptimizationStrategy::MINIMIZE: {
#pragma omp parallel for
                for (int i = 0; i < this->numberOfAgents; i++) {
                    if (newIndividuals1Fitness[i] < newIndividuals2Fitness[i] && newIndividuals1Fitness[i] < individualsFitness[i]) {
                        individuals[i] = newIndividuals1[i];
                    } else if (newIndividuals2Fitness[i] < newIndividuals1Fitness[i] && newIndividuals2Fitness[i] < individualsFitness[i]) {
                        individuals[i] = newIndividuals2[i];
                    }
                }
            } break;
            case OptimizationStrategy::MAXIMIZE: {
#pragma omp parallel for
                for (int i = 0; i < this->numberOfAgents; i++) {
                    if (newIndividuals1Fitness[i] > newIndividuals2Fitness[i] && newIndividuals1Fitness[i] > individualsFitness[i]) {
                        individuals[i] = newIndividuals1[i];
                    } else if (newIndividuals2Fitness[i] > newIndividuals1Fitness[i] && newIndividuals2Fitness[i] > individualsFitness[i]) {
                        individuals[i] = newIndividuals2[i];
                    }
                }
            } break;
            }
        }

        std::cout << "Best solution " << this->globalBestFitness << " Running time: " << utils::getCurrentTime() << std::endl << "Best solution decision variables: ";
        utils::printVector(this->globalBest);
    }

    void crossover(std::vector<T> const &parent1, std::vector<T> const &parent2, std::vector<T> &child1, std::vector<T> &child2)
    {
        switch (this->crossoverType) {
        case CrossoverType::UNIFORM:
            uniformCrossover(parent1, parent2, child1, child2);
            break;
        case CrossoverType::SIMULATED_BINARY:
            simulatedBinaryCrossover(parent1, parent2, child1, child2);
            break;
        case CrossoverType::ONE_POINT:
            std::cout << "One Point Crossover" << std::endl;
            break;
        }
    }

    void simulatedBinaryCrossover(const std::vector<T> &parent1, const std::vector<T> &parent2, std::vector<T> &child1, std::vector<T> &child2)
    {
        double EPS = 1.0e-14;
        // y1 stores the value for the 1st child; y2 the value for the 2nd child; yl (notice
        // it's an L not a 1) holds the lower limit for the variable yu holds the upper limit
        double y1, y2, yl, yu;
        // betaq, in the paper, is the symbol beta with a line above
        double alpha, beta, betaq;

        if (utils::getRandom() <= this->crossoverRate) // roll the dices. Should we apply a crossover?
        {
            // Crossover operations for the MS crossover
            for (int i = 0; i < this->problem->getDimension(); i++) // for each variable of a solution (individual)
            {
                // according to the paper, each variable in a solution has a 50%
                // chance of changing its value. This should be removed when dealing
                // with one-dimensional solutions.
                if (utils::getRandom() <= 0.5) {
                    // the following if/else block puts the lowest value between
                    // parent1 and parent2 in y1 and the other in y2.
                    if (parent1[i] < parent2[i]) {
                        y1 = parent1[i];
                        y2 = parent2[i];
                    } else {
                        y1 = parent2[i];
                        y2 = parent1[i];
                    }
                    // if the value in parent1 is not the same of parent2
                    if (fabs(parent1[i] - parent2[i]) > EPS) {
                        yl = this->problem->getLb()[i]; // lower limit of the i-th
                                        // variable of an individual.
                                        // Note that yl != y1.
                        yu = this->problem->getUb()[i]; // upper limit of the i-th
                                        // variable of an individual
                        // Calculation for the first child
                        double rand = utils::getRandom();
                        beta = 1.0 + (2.0 * (y1 - yl) / (y2 - y1)); // it differs from the paper here. The
                                                // paper uses one value of beta for
                                                // calculating both children. Here, we
                                                // use one beta for each child.
                        alpha = 2.0 - pow(beta,
                                  -(etaC + 1.0)); // calculation of alpha as
                                          // described in the paper
                        // calculation of betaq as described in the paper
                        if (rand <= (1.0 / alpha)) {
                            betaq = pow((rand * alpha), (1.0 / (etaC + 1.0)));
                        } else {
                            betaq = pow((1.0 / (2.0 - rand * alpha)), (1.0 / (etaC + 1.0)));
                        }
                        child1[i] = 0.5 * ((y1 + y2) - betaq * (y2 - y1)); // calculation of the first child as
                                                   // described in the paper
                        // Calculations for the second child
                        beta = 1.0 + (2.0 * (yu - y2) / (y2 - y1)); // differs from the paper. The
                                                // second value of beta uses the
                                                // upper limit (yu) and the maximum
                                                // between parent1 and parent2 (y2)
                        alpha = 2.0 - pow(beta,
                                  -(etaC + 1.0)); // calculation of alpha as
                                          // described in the paper
                        // calculation of betaq as described in the paper
                        if (rand <= (1.0 / alpha)) {
                            betaq = pow((rand * alpha), (1.0 / (etaC + 1.0)));
                        } else {
                            betaq = pow((1.0 / (2.0 - rand * alpha)), (1.0 / (etaC + 1.0)));
                        }
                        child2[i] = 0.5 * ((y1 + y2) + betaq * (y2 - y1)); // calculation of the second child
                                                   // as described in the paper ensures
                                                   // the value of both children are in
                                                   // the correct bounds [yl, yu].
                                                   // According to the paper, this
                                                   // should not be needed.
                    }
                    // if the i-th variable has the same value in both parents
                    else {
                        child1[i] = parent1[i];
                        child2[i] = parent2[i];
                    }
                } else // 50% chance of changing values. In the case random > 0.5,
                       // the children should have the same value as the parents
                {
                    child1[i] = parent1[i];
                    child2[i] = parent2[i];
                }
            }
        } else {
            child1 = parent1;
            child2 = parent2;
        }
    }

    void uniformCrossover(const std::vector<T> &parent1, const std::vector<T> &parent2, std::vector<T> &child1, std::vector<T> &child2)
    {
        for (size_t i = 0; i < this->problem->getDimension(); i++) {
            if (utils::getRandom() <= 0.5) {
                child1[i] = parent1[i];
                child2[i] = parent2[i];
            } else {
                child1[i] = parent2[i];
                child2[i] = parent1[i];
            }
        }
    }

    void selection(int &indexParent1, int &indexParent2, const std::vector<double> &individualsFitness)
    {
        switch (this->selectionType) {
        case SelectionType::FITNESS_PROPORTIONATE: {
            std::cout << "Fitness proportionate" << std::endl;
        } break;
        case SelectionType::TOURNAMENT: {
            indexParent1 = tournamentSelection(individualsFitness);
            indexParent2 = tournamentSelection(individualsFitness);
        } break;
        }
    }

    int tournamentSelection(const std::vector<double> &individualsFitness)
    {
        int indexIndividual1 = utils::getRandom(this->numberOfAgents - 1);
        int indexIndividual2 = utils::getRandom(this->numberOfAgents - 2);
        if (indexIndividual2 >= indexIndividual1)
            indexIndividual2++;

        if (individualsFitness[indexIndividual1] > individualsFitness[indexIndividual2])
            return indexIndividual1;
        else
            return indexIndividual2;
    }

    void mutate(std::vector<double> &child1, std::vector<double> &child2)
    {
        switch (this->mutationType) {
        case MutationType::RANDOM_MUTATION: {
            randomMutation(child1);
            randomMutation(child1);
        } break;
        case MutationType::POLYNOMIAL: {
            polynomialMutation(child1);
            polynomialMutation(child2);
        } break;
        }
    }

    void randomMutation(std::vector<double> &individual)
    {
        for (int i = 0; i < this->problem->getDimension(); i++)
            if (utils::getRandom() <= mutationRate)
                individual[i] = this->problem->getRandomDecisionVariableAt(i);
    }

    void polynomialMutation(std::vector<double> &individual)
    {
        for (int i = 0; i < this->problem->getDimension(); i++) {
            if (utils::getRandom() <= mutationRate) {
                double rand = utils::getRandom();
                if (rand <= 0.5) {
                    double leftValue = individual[i] - this->problem->getLb()[i];
                    double sigma_l = pow(2 * rand, 1. / (1 + etaM)) - 1;
                    individual[i] = individual[i] + (sigma_l * leftValue);
                } else {
                    double rightValue = this->problem->getUb()[i] - individual[i]; // 1 is the upper bound
                                                       // for the ith variable
                    double sigma_r = 1 - pow(2 * (1 - rand), 1. / (1 + etaM));
                    individual[i] = individual[i] + (sigma_r * rightValue);
                }
            }
        }
    }

    void setEtaM(float value) { etaM = value; }
    void setEtaC(float value) { etaC = value; }

private:
    float crossoverRate;
    float mutationRate;
    float etaM = -FLT_MIN;
    float etaC = -FLT_MIN;
    CrossoverType crossoverType;
    SelectionType selectionType;
    MutationType mutationType;
};

#endif // GENETICALGORITHM_H
