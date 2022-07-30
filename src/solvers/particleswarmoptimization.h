#ifndef PARTICLESWARMOPTIMIZATION_H
#define PARTICLESWARMOPTIMIZATION_H

#include "../problem.h"
#include "globalsolver.h"
#include <iostream>
#include <memory>

template <class T>
class ParticleSwarmOptimization : public GlobalSolver<T>
{
public:
    ParticleSwarmOptimization(int numberOfParticles, double c1, double c2, double wMin, double wMax, double vMax, std::shared_ptr<Problem<T>> prob)
        : GlobalSolver<T>(numberOfParticles, prob)
    {
        if (this->numberOfAgents < 4) {
            std::cerr << "The number of particles needs to be equal or higher than 4" << std::endl;
            exit(EXIT_FAILURE);
        }

        this->c1 = c1;
        this->c2 = c2;
        this->wMin = wMin;
        this->wMax = wMax;
        this->vMax = std::vector<double>(this->problem->getDimension());
        for (size_t i = 0; i < this->problem->getDimension(); i++) {
            //            vMax[i] = 0.5 * (this->problem->getUb()[i] - this->problem->getLb()[i]);
            this->vMax[i] = vMax;
        }

        puts("ParticleSwarmOptimiza[tion instantiated");
    }

    void solve()
    {
        if (this->maxIterations == 0 && this->runningTime == 0) {
            std::cerr << "Use \"setMaxIterations(int)\" or \"setRunningTime(double)\" to "
                "define a stopping criteria!"
                 << std::endl;
            exit(EXIT_FAILURE);
        } else
            std::cout << "Starting ParticleSwarmOptimization search procedure" << std::endl;

        utils::startTimeCounter();

        // Current population
        std::cout << this->numberOfAgents << std::endl;
        std::vector<std::vector<double>> particles(this->numberOfAgents);

        // Define random starting positions
        for (size_t i = 0; i < this->numberOfAgents; i++)
            this->problem->fillRandomDecisionVariables(particles[i]);

        // Evaluate particles
        std::vector<double> particlesFitness(this->numberOfAgents);
        for (size_t i = 0; i < this->numberOfAgents; i++) {
            // this->problem->current = utils::getCurrentTime();
            switch (this->problem->getRepType()) {
            case RepresentationType::DIRECT:
                particlesFitness[i] = this->problem->evaluate(particles[i]);
                break;
            case RepresentationType::INDIRECT:
                std::shared_ptr<Solution<T>> sol = this->problem->construct(particles[i]);
                particlesFitness[i] = sol->getFitness();
                break;
            }
            this->updateGlobalBest(particles[i], particlesFitness[i], true);
        }

        // Keep track of the best position of each particle and
        std::vector<std::vector<double>> particlesBestPosition = particles;
        std::vector<double> particlesBestFitness = particlesFitness;

        // Velocity of each particle
        std::vector<std::vector<double>> velocities(this->numberOfAgents, std::vector<double>(this->problem->getDimension(), 0));

        int iteration = -1;
        while (iteration++ < this->maxIterations || utils::getCurrentTime() < this->runningTime) {
            double w;
            if (iteration < this->maxIterations)
                w = wMax - iteration * ((wMax - wMin) / this->maxIterations);
            else
                w = wMax - utils::getCurrentTime() * ((wMax - wMin) / this->runningTime);

#pragma omp parallel for
            for (int i = 0u; i < this->numberOfAgents; i++) {
                for (size_t j = 0; j < this->problem->getDimension(); j++) {
                    double r1 = utils::getRandom();
                    double r2 = utils::getRandom();

                    const double movTowardsPersonal = c1 * r1 * (particlesBestPosition[i][j] - particles[i][j]);
                    const double movTowardsGlobal = c2 * r2 * (this->globalBest[j] - particles[i][j]);
                    velocities[i][j] = std::max(-vMax[i], std::min(vMax[i], w * velocities[i][j] + movTowardsPersonal + movTowardsGlobal));
                    particles[i][j] = std::max(this->problem->getLb()[j], std::min(this->problem->getUb()[j], particles[i][j] + velocities[i][j]));
                }
            }

#pragma omp parallel for
            for (size_t i = 0; i < this->numberOfAgents; i++) {
                switch (this->problem->getRepType()) {
                case RepresentationType::DIRECT:
                    particlesFitness[i] = this->problem->evaluate(particles[i]);
                    break;
                case RepresentationType::INDIRECT:
                    std::shared_ptr<Solution<T>> sol = this->problem->construct(particles[i]);
                    particlesFitness[i] = sol->getFitness();
                    break;
                }

                if (particlesBestFitness[i] < particlesFitness[i]) {
                    particlesBestPosition[i] = particles[i];
                    particlesBestFitness[i] = particlesFitness[i];
                }

#pragma omp critical
                this->updateGlobalBest(particles[i], particlesFitness[i], true);
            }
        }
    }

private:
    double c1, c2, wMax, wMin;
    std::vector<double> vMax;
};

#endif // PARTICLESWARMOPTIMIZATION_H
