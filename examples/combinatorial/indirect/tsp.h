#ifndef TSP_H
#define TSP_H

#include <algorithm>
#include <map>
#include <memory>
#include <numeric>
#include <string>
#include <tuple>

#include "../../../entities/problem.h"

typedef double encoding;

namespace tsp
{ // TSP problem related structures
struct Vector2 {
    float x;
    float y;

    Vector2(double x, double y)
    {
        this->x = x;
        this->y = y;
    }
};

std::vector<std::shared_ptr<Vector2>> nodes;
std::map<std::string, int> distances;

double euclideanDistance(std::shared_ptr<Vector2> node1, std::shared_ptr<Vector2> node2) { return sqrt(pow(node2->x - node1->x, 2) + pow(node2->y - node1->y, 2)); }

void createNodes(const std::vector<std::pair<int, int>> rawNodes)
{
    nodes = std::vector<std::shared_ptr<Vector2>>(rawNodes.size());
    for (size_t i = 0; i < rawNodes.size(); i++)
        nodes[i] = std::shared_ptr<Vector2>(new Vector2(rawNodes[i].first, rawNodes[i].second));
}

std::string composeKey(std::shared_ptr<Vector2> node1, std::shared_ptr<Vector2> node2)
{
    return std::to_string(node1->x) + "," + std::to_string(node1->y) + "," + std::to_string(node2->x) + "," + std::to_string(node2->y);
}

void calculateDistances()
{
    for (size_t i = 0; i < nodes.size(); i++)
        for (size_t j = 0; j < nodes.size(); j++)
            if (i != j) {
                int distance = std::round(euclideanDistance(nodes[i], nodes[j]));
                std::string strCoordinates = composeKey(nodes[i], nodes[j]);
                if (!distances.count(strCoordinates))
                    distances[strCoordinates] = std::round(distance);
            }
}
} // namespace tsp

class TSPSolution : public Solution<encoding>
{
public:
    TSPSolution(int dimension, std::vector<encoding> &decVar) : Solution(decVar)
    {
        this->dimension = dimension;
        this->fitness = 0;
        this->createPermutation(decVar);
        this->calculateFitness();
        // std::cout << fitness << std::endl;
        // localSearch();
    }

    void localSearch() override
    {
        for (size_t i = 0; i < dimension; i++) {
            for (size_t j = i; j < dimension; j++) {
                if (i != j) {
                    std::vector<int> newPermutation = permutation;
                    std::swap(newPermutation[i], newPermutation[j]);
                }
            }
        }
    }

    void print() override
    {
        std::cout << "Path: { ";
        for (size_t i = 0; i < dimension; i++) {
            int index = permutation[i];
            if (i + 1 < dimension)
                std::cout << "[" << tsp::nodes[index]->x << "," << tsp::nodes[index]->y << "]->";
            else
                std::cout << "[" << tsp::nodes[index]->x << "," << tsp::nodes[index]->y << "] }" << std::endl;
        }
    }

protected:
    int dimension;
    std::vector<int> permutation; // stores the permutation of the nodes, i.e., the order in which the nodes are visited

    void createPermutation(const std::vector<encoding> &decisionVariables)
    {
        permutation = std::vector<int>(dimension);
        std::iota(permutation.begin(), permutation.end(), 0);
        std::sort(permutation.begin(), permutation.end(),
              [&](int pos1, int pos2) { return std::tie(decisionVariables[pos1], pos1) < std::tie(decisionVariables[pos2], pos2); });
    }

    void calculateFitness()
    {
        this->fitness = 0;
        // Sum of the edges' weight of the path
        for (size_t i = 0; i < dimension - 1; i++) {
            int index1 = permutation[i];
            int index2 = permutation[i + 1];

            std::shared_ptr<tsp::Vector2> node1 = tsp::nodes[index1];
            std::shared_ptr<tsp::Vector2> node2 = tsp::nodes[index2];
            std::string key = tsp::composeKey(node1, node2);
            int distance = tsp::distances[key];
            this->fitness += distance;
        }

        // Add the weight from the last to the first, i.e. ciclic
        std::shared_ptr<tsp::Vector2> node1 = tsp::nodes[0];
        std::shared_ptr<tsp::Vector2> node2 = tsp::nodes[dimension - 1];
        std::string key = tsp::composeKey(node1, node2);
        int distance = tsp::distances[key];
        this->fitness += distance;
    }
};

class TravellingSalesmanProblem : public Problem<encoding>
{
public:
    TravellingSalesmanProblem(int dimension, const std::vector<std::pair<int, int>> rawNodes, OptimizationStrategy strategy, RepresentationType repType)
        : Problem(strategy, repType)
    {
        this->lb = std::vector<encoding>(dimension);
        this->ub = std::vector<encoding>(dimension);
        this->dimension = dimension;

        for (size_t i = 0; i < this->dimension; i++) {
            lb[i] = 0;
            ub[i] = nextafter(1.0, 0.0);
        }

        tsp::createNodes(rawNodes);
        tsp::calculateDistances();
    }

    std::shared_ptr<Solution<double>> construct(std::vector<encoding> &decisionVariables) override
    {
        std::shared_ptr<TSPSolution> solution(new TSPSolution(this->dimension, decisionVariables));
        numbTriedSolution++;
        return solution;
    }
};

#endif // TSP_H
