#ifndef TSP_H
#define TSP_H

#include "../../../entities/problem.h"
#include <memory>
#include <cmath>

typedef double encoding;

struct Vector2 {
    float x;
    float y;

    Vector2(double x, double y)
    {
        this->x = x;
        this->y = y;
    }
};

class TSPSolution : public Solution<encoding>
{
public:
    TSPSolution(int dimension, std::vector<encoding> &decVar) : Solution(decVar)
    {
        this->dimension = dimension;

        if (utils::getRandom() <= 0.0001)
            this->localSearch();

        double A = 10, sum = 0;
        for (size_t i = 0; i < dimension; i++) {
            sum += pow(decisionVariables[i], 2) - A * cos(2 * M_PI * decisionVariables[i]);
        }
        this->fitness = A * dimension + sum;
    }

    void localSearch() override
    {
        for (size_t i = 0; i < dimension; i++)
            this->decisionVariables[i] = 0;
    }

protected:
    int dimension;
};


class TSP : public Problem<encoding>
{
public:
    TSP(int dimension, std::vector<double> xnodes, std::vector<double> ynodes, OptimizationStrategy strategy, RepresentationType repType) : Problem(strategy, repType)
    {
        this->lb = std::vector<encoding>(dimension);
        this->ub = std::vector<encoding>(dimension);
        this->dimension = dimension;

        for (size_t i = 0; i < this->dimension; i++) {
            lb[i] = 0;
            ub[i] = nextafter(1.0, 0.0);
        }

        nodes = std::vector<std::shared_ptr<Vector2>>(dimension);
        for (size_t i = 0; i < this->dimension; i++)
            nodes[i] = std::shared_ptr<Vector2>(new Vector2(xnodes[i], ynodes[i]));
    }

    std::shared_ptr<Solution<double>> construct(std::vector<encoding> &decisionVariables) override
    {
        std::shared_ptr<TSPSolution> solution(new TSPSolution(this->dimension, decisionVariables));
        return solution;
    }

private:
    std::vector<std::shared_ptr<Vector2>> nodes;

    double euclideanDistance(std::shared_ptr<Vector2> node1, std::shared_ptr<Vector2> node2) {
        return sqrt(pow(node2->x - node1->x, 2) + pow(node2->y - node1->y, 2));
    }
};

#endif // TSP_H
