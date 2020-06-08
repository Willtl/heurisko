#ifndef PARTICLESWARMOPTIMIZATION_H
#define PARTICLESWARMOPTIMIZATION_H

#include "../entities/problem.h"
#include "globalsolver.h"
#include <iostream>
#include <memory>

using namespace std;

template <class T>
class ParticleSwarmOptimization : public GlobalSolver<T>
{
public:
    ParticleSwarmOptimization(int numberOfParticles, double c1, double c2, double wMin, double wMax, shared_ptr<Problem<T>> prob) : GlobalSolver<T>(numberOfParticles, prob)
    {
        if (this->numberOfAgents < 4) {
            cerr << "The number of particles needs to be equal or higher than 4" << endl;
            exit(EXIT_FAILURE);
        }

        this->c1 = c1;
        this->c2 = c2;
        this->wMin = wMin;
        this->wMax = wMax;
        this->vMax = vector<double>(this->problem->getDimension());
        for (size_t i = 0; i < this->problem->getDimension(); i++) {
            vMax[i] = 0.5 * (this->problem->getUb()[i] - this->problem->getLb()[i]);
        }

        puts("ParticleSwarmOptimiza[tion instantiated");
    }

    void solve()
    {
        if (this->maxIterations == 0 && this->runningTime == 0) {
            cerr << "Use \"setMaxIterations(int)\" or \"setRunningTime(double)\" to define a stopping criteria!" << endl;
            exit(EXIT_FAILURE);
        } else
            cout << "Starting ParticleSwarmOptimization search procedure" << endl;

        utils::startTimeCounter();

        // Current population
        cout << this->numberOfAgents << endl;
        vector<vector<double>> particles(this->numberOfAgents);

        // Define random starting positions
        for (size_t i = 0; i < this->numberOfAgents; i++)
            this->problem->fillRandomDecisionVariables(particles[i]);

        // Evaluate particles
        vector<double> particlesFitness(this->numberOfAgents);
        for (size_t i = 0; i < this->numberOfAgents; i++) {
            this->problem->current = utils::getCurrentTime();
            particlesFitness[i] = this->problem->evaluate(particles[i]);
            this->updateGlobalBest(particles[i], particlesFitness[i], true);
        }

        // Keep track of the best position of each particle and
        vector<vector<double>> particlesBestPosition = particles;
        vector<double> particlesBestFitness = particlesFitness;

        // Velocity of each particle
        vector<vector<double>> velocities(this->numberOfAgents, vector<double>(this->problem->getDimension(), 0));

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
                particlesFitness[i] = this->problem->evaluate(particles[i]);
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
    vector<double> vMax;
};

#endif // PARTICLESWARMOPTIMIZATION_H
