#ifndef GREYWOLFOPTIMIZER_H
#define GREYWOLFOPTIMIZER_H

#include "../problem.h"
#include "globalsolver.h"
#include <iostream>
#include <memory>

template <class T>
class GreyWolfOptimizer : public GlobalSolver<T>
{
public:
    GreyWolfOptimizer(int numberOfAgents, std::shared_ptr<Problem<T>> prob) : GlobalSolver<T>(numberOfAgents, prob)
    {
        if (this->numberOfAgents < 3) {
            std::cerr << "The number of individuals needs to be equal or higher than 3" << std::endl;
            exit(EXIT_FAILURE);
        }

        puts("Grey Wolf Optimizer instantiated");
    }

    void solve()
    {
        if (this->maxIterations == 0 && this->runningTime == 0) {
            std::cerr << "Use \"setMaxIterations(int)\" or \"setRunningTime(double)\" to "
                     "define a stopping criteria!"
                  << std::endl;
            exit(EXIT_FAILURE);
        } else
            std::cout << "Grey Wolf Optimizer search procedure" << std::endl;

        utils::startTimeCounter();

        // Current population
        std::cout << this->numberOfAgents << std::endl;
        std::vector<std::vector<double>> wolves(this->numberOfAgents);
#pragma omp parallel for
        for (size_t i = 0; i < this->numberOfAgents; i++)
            this->problem->fillRandomDecisionVariables(wolves[i]);

        // Stores the objective value of each individual
        std::vector<double> wolvesFitness(this->numberOfAgents);
#pragma omp parallel for
        for (size_t i = 0; i < this->numberOfAgents; i++) {
            switch (this->problem->getRepType()) {
            case RepresentationType::DIRECT:
                wolvesFitness[i] = this->problem->evaluate(wolves[i]);
                break;
            case RepresentationType::INDIRECT:
                std::shared_ptr<Solution<T>> sol = this->problem->construct(wolves[i]);
                wolvesFitness[i] = sol->getFitness();
                break;
            }

#pragma omp critical
            this->updateGlobalBest(wolves[i], wolvesFitness[i], true);
        }

        std::vector<double> alpha, beta, delta;
        double fitnessAlpha, fitnessBeta, fitnessDelta;

        int iteration = -1;
        while (iteration++ < this->maxIterations || utils::getCurrentTime() < this->runningTime) {
            for (size_t i = 0; i < this->numberOfAgents; i++) {
                if (alpha.empty() || wolvesFitness[i] < fitnessAlpha) {
                    alpha = wolves[i];
                    fitnessAlpha = wolvesFitness[i];
                } else if (beta.empty() || wolvesFitness[i] < fitnessBeta) {
                    beta = wolves[i];
                    fitnessBeta = wolvesFitness[i];
                } else if (delta.empty() || wolvesFitness[i] < fitnessDelta) {
                    delta = wolves[i];
                    fitnessDelta = wolvesFitness[i];
                }
            }

            // Decreases linearly from 2 to 0
            double a;
            if (iteration < this->maxIterations)
                a = 2 - iteration * (2. / this->maxIterations);
            else
                a = 2 - utils::getCurrentTime() * (2. / this->runningTime);

#pragma omp parallel for
            for (size_t i = 0; i < this->numberOfAgents; i++) {
                for (size_t j = 0; j < this->problem->getDimension(); j++) {
                    double r1 = utils::getRandom();
                    double r2 = utils::getRandom();
                    double A1 = 2 * a * r1 - a;
                    double C1 = 2 * r2;
                    const double dAlpha = abs(C1 * alpha[j] - wolves[i][j]);
                    const double X1 = alpha[j] - A1 * dAlpha;

                    r1 = utils::getRandom();
                    r2 = utils::getRandom();
                    double A2 = 2 * a * r1 - a;
                    double C2 = 2 * r2;
                    const double dBeta = std::abs(C2 * beta[j] - wolves[i][j]);
                    const double X2 = beta[j] - A2 * dBeta;

                    r1 = utils::getRandom();
                    r2 = utils::getRandom();
                    double A3 = 2 * a * r1 - a;
                    double C3 = 2 * r2;
                    const double dDelta = std::abs(C3 * delta[j] - wolves[i][j]);
                    const double X3 = delta[j] - A3 * dDelta;

                    wolves[i][j] = max(this->problem->getLb()[j], min(X1 + X2 + X3 / 3, this->problem->getUb()[j]));
                }
            }

#pragma omp parallel for
            for (size_t i = 0; i < this->numberOfAgents; i++) {
                switch (this->problem->getRepType()) {
                case RepresentationType::DIRECT:
                    wolvesFitness[i] = this->problem->evaluate(wolves[i]);
                    break;
                case RepresentationType::INDIRECT:
                    std::shared_ptr<Solution<T>> sol = this->problem->construct(wolves[i]);
                    wolvesFitness[i] = sol->getFitness();
                    break;
                }
#pragma omp critical
                this->updateGlobalBest(wolves[i], wolvesFitness[i], true);
            }
        }
        std::cout << "Best solution " << this->globalBestFitness << " Running time: " << utils::getCurrentTime() << std::endl << "Best solution decision variables: ";
        utils::printVector(this->globalBest);
    }
};
#endif // GREYWOLFOPTIMIZER_H
