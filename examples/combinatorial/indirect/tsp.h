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
    int id;
    float x;
    float y;

    Vector2(int id, double x, double y)
    {
        this->id = id;
        this->x = x;
        this->y = y;
    }
};

std::vector<std::shared_ptr<Vector2>> nodes;
std::vector<std::vector<int>> distanceMatrix;

double euclideanDistance(std::shared_ptr<Vector2> node1, std::shared_ptr<Vector2> node2) { return sqrt(pow(node2->x - node1->x, 2) + pow(node2->y - node1->y, 2)); }

void createNodes(const std::vector<std::pair<int, int>> rawNodes)
{
    nodes = std::vector<std::shared_ptr<Vector2>>(rawNodes.size());
    for (size_t i = 0; i < rawNodes.size(); i++)
        nodes[i] = std::shared_ptr<Vector2>(new Vector2(i, rawNodes[i].first, rawNodes[i].second));
}

void calculateDistances()
{
    distanceMatrix = std::vector<std::vector<int>>(nodes.size());
    for (size_t i = 0; i < nodes.size(); i++)
        distanceMatrix[i] = std::vector<int>(nodes.size());

    for (size_t i = 0; i < nodes.size(); i++)
        for (size_t j = 0; j < nodes.size(); j++)
            if (i == j)
                distanceMatrix[i][j] = 0;
            else {
                int distance = std::round(euclideanDistance(nodes[i], nodes[j]));
                distanceMatrix[i][j] = distance;
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
    }

    // 2-opt Swap mechanism
    // Reverse the direction of the path between indexes start and end
    void twoOptSwap(std::vector<int> &permutation, const int start, const int end, std::vector<int> &newPermutation)
    {
        for (int i = 0; i < start; i++)
            newPermutation[i] = permutation[i];
        int counter = start;
        for (int i = end; i >= start; i--) {
            newPermutation[counter] = permutation[i];
            counter++;
        }
        for (int i = end + 1; i < permutation.size(); i++)
            newPermutation[i] = permutation[i];
    }

    // This 2-opt neighborhood function
    void localSearch() override
    {
        // Stores the best solution in the neighborhood
        std::shared_ptr<TSPSolution> bestNeighbor = NULL;
        while (true) {
            std::cout << "current fitness " << fitness << std::endl;
            // Check every neighbor solution obtaining by swapping elements of the permutation
            for (size_t i = 0; i < dimension - 1; i++) {
                for (size_t j = i + 1; j < dimension; j++) {
                    std::cout << i << "," << j << std::endl;

                    // Define Neighbor solution by inverting the path between two points
                    std::vector<int> newPermutation(permutation.size());
                    twoOptSwap(permutation, i, j, newPermutation);
                    // Decision variables values of the neighbor solution
                    std::vector<double> newDecisionVariables(dimension, 0);
                    // Encode permutation into continuous domain
                    double share = 1.0 / dimension, acc = 0;
                    for (size_t k = 0; k < dimension; k++) {
                        const int index = newPermutation[k];
                        newDecisionVariables[index] = acc;
                        acc += share;
                    }
                    // Construct neighbor solution
                    std::shared_ptr<TSPSolution> neighbor(new TSPSolution(this->dimension, newDecisionVariables));
                    // Check if the current neighbor is the best neighbor
                    if (bestNeighbor == NULL || bestNeighbor->fitness > neighbor->fitness)
                        bestNeighbor = neighbor;
                }
            }

            // Replace current solution if the best neighbor is better, else stop local search
            if (bestNeighbor != NULL && bestNeighbor->fitness < this->fitness) {
                this->fitness = bestNeighbor->fitness;
                this->permutation = bestNeighbor->permutation;
                this->decisionVariables = bestNeighbor->decisionVariables;
                break;
            } else
                break;
        }
    }

    // This uses a simple swapping neighborhood function
    void swapLocalSearch()
    {
        // Stores the best solution in the neighborhood
        std::shared_ptr<TSPSolution> bestNeighbor = NULL;
        while (true) {
            // Check every neighbor solution obtaining by swapping elements of the permutation
            for (size_t i = 0; i < dimension; i++) {
                for (size_t j = i; j < dimension; j++) {
                    if (i != j) {
                        // The only advantage of this neighborhood is its simplicity
                        // Define Neighbor solution by SIMPLY swapping two elements in the permutation
                        std::vector<int> newPermutation = permutation;
                        std::swap(newPermutation[i], newPermutation[j]);
                        // Decision variables values of the neighbor solution
                        std::vector<double> newDecisionVariables(dimension, 0);
                        // Encode permutation into continuous domain
                        double share = 1.0 / dimension, acc = 0;
                        for (size_t k = 0; k < dimension; k++) {
                            const int index = newPermutation[k];
                            newDecisionVariables[index] = acc;
                            acc += share;
                        }
                        // Construct neighbor solution
                        std::shared_ptr<TSPSolution> neighbor(new TSPSolution(this->dimension, newDecisionVariables));
                        // Check if the current neighbor is the best neighbor
                        if (bestNeighbor == NULL || bestNeighbor->fitness > neighbor->fitness)
                            bestNeighbor = neighbor;
                    }
                }
            }

            // Replace current solution if the best neighbor is better, else stop local search
            if (bestNeighbor != NULL && bestNeighbor->fitness < this->fitness) {
                this->fitness = bestNeighbor->fitness;
                this->permutation = bestNeighbor->permutation;
                this->decisionVariables = bestNeighbor->decisionVariables;
                break;
            } else
                break;
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
                std::cout << "[" << tsp::nodes[index]->x << "," << tsp::nodes[index]->y << "] }\n";
        }
    }

    std::vector<int> getPermutation() const { return permutation; }

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
            int distance = tsp::distanceMatrix[index1][index2];
            this->fitness += distance;
        }

        // Add the weight from the last to the first, i.e. ciclic
        int distance = tsp::distanceMatrix[0][dimension - 1];
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
        // solution->localSearch();
        numbTriedSolution++;
        return solution;
    }
};

#endif // TSP_H
