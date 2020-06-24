#ifndef VRP_H
#define VRP_H

#include <algorithm>
#include <map>
#include <memory>
#include <numeric>
#include <string>
#include <tuple>

#include "../../../entities/problem.h"

typedef double encoding;

namespace vrp
{
int depot;
int numberVehicles;
std::vector<std::vector<double>> distanceMatrix;
} // namespace vrp

class VRPSolution : public Solution<encoding>
{
public:
    VRPSolution(const int dimension, std::vector<encoding> &decVar) : Solution(dimension, decVar)
    {
        this->fitness = 0;
        this->vehicleRoutes = std::vector<std::vector<int>>(vrp::numberVehicles);

        // Create an permutation of the nodes
        this->createPermutation(decVar);

        // Define assignment of the nodes to the vehicles
        this->createAssignment(decVar);

        // utils::printVector<std::vector<int>>(this->assigment);
        // utils::printVector<std::vector<int>>(this->permutation);

        this->calculateFitness2();
    }

    void print() override
    {
        ps << "Obj. value: " << this->fitness << pe;

        std::cout << "Assignment: ";
        for (size_t i = 0; i < this->dimension / 2; i++) {
            int index = this->assigment[i];
            if (i + 1 < this->dimension / 2)
                std::cout << index << ", ";
            else
                std::cout << index << "\n";
        }

        std::cout << "Permutation: ";
        for (size_t i = 0; i < this->dimension / 2; i++) {
            int index = permutation[i];
            if (i + 1 < this->dimension / 2)
                std::cout << index << ", ";
            else
                std::cout << index << "\n";
        }
    }

protected:
    std::vector<int> permutation; // stores the permutation of the nodes, i.e., the order in which the nodes are visited
    std::vector<int> assigment;   // stores the assignment of the nodes to the vehicles
    std::vector<std::vector<int>> vehicleRoutes;

    void createPermutation(const std::vector<encoding> &decisionVariables)
    {
        permutation = std::vector<int>(this->dimension / 2);
        for (int i = 0; i < this->dimension / 2; i++)
            permutation[i] = i;
        std::sort(permutation.begin(), permutation.end(),
              [&](int pos1, int pos2) { return std::tie(decisionVariables[pos1], pos1) < std::tie(decisionVariables[pos2], pos2); });
    }

    void createAssignment(const std::vector<encoding> &decisionVariables)
    {
        // floor(cont_ms_chromo[i] * linear_operations[i]->resources.size());
        assigment = std::vector<int>(this->dimension / 2);
        int index = 0;
        for (int i = this->dimension / 2; i < this->dimension; i++) {
            assigment[index] = std::floor(decisionVariables[i] * vrp::numberVehicles);
            index++;
        }
    }

    // Not much difference in terms of performance but calculateFitness2
    // is more reliable and leads to better performance overall
    void calculateFitness1()
    {
        // Computes the distance of the route of each vehicle and keep the max
        int prevIndex, currIndex;
        int maxDistance = 0, currentDistance;
        for (int i = 0; i < vrp::numberVehicles; i++) {
            prevIndex = -1, currIndex = -1, currentDistance = 0;
            for (int j = 0; j < this->permutation.size(); j++) {
                const int nodeIndex = this->permutation[j];
                if (nodeIndex != vrp::depot) {
                    const int assignedVehicle = this->assigment[nodeIndex];
                    if (i == assignedVehicle && prevIndex == -1) {
                        prevIndex = nodeIndex;
                        // From depot to first node
                        int depotIndex = vrp::depot;
                        currentDistance += vrp::distanceMatrix[depotIndex][prevIndex];
                    } else if (i == assignedVehicle && currIndex == -1)
                        currIndex = nodeIndex;

                    // Compute length between path from prev to curr
                    if (i == assignedVehicle && prevIndex != -1 && currIndex != -1) {
                        currentDistance += vrp::distanceMatrix[prevIndex][currIndex];
                        prevIndex = currIndex;
                        currIndex = -1;
                    }
                }
                // From last to depot
                if (j + 1 == permutation.size() && prevIndex != -1) {
                    const int depotIndex = vrp::depot;
                    const int lastIndex = prevIndex;
                    currentDistance += vrp::distanceMatrix[depotIndex][lastIndex];
                }
            }
            maxDistance = std::max(maxDistance, currentDistance);
        }
        this->fitness = maxDistance;
    }

    // Decoding strategy two
    void calculateFitness2()
    {
        // Initialize route sub-permutation of each vehicle and reserve (dimension/2) to avoid resize
        for (size_t i = 0; i < vrp::numberVehicles; i++) {
            vehicleRoutes[i] = std::vector<int>();
            vehicleRoutes[i].reserve(dimension / 2);
        }

        // Create the route (sub-permutation) of each vehicle
        for (size_t i = 0; i < permutation.size(); i++) {
            int nodeIndex = permutation[i];
            int vehiIndex = assigment[nodeIndex];
            // All vehicles leaves and return to depot
            if (nodeIndex != vrp::depot)
                vehicleRoutes[vehiIndex].emplace_back(nodeIndex);
        }

        // Computes the distance of the route of each vehicle and keep the max
        int maxDistance = 0, currentDistance;
        for (size_t i = 0; i < vrp::numberVehicles; i++) {
            if (vehicleRoutes[i].size() > 0) {
                currentDistance = 0;
                // From depot to first node
                int index1 = vrp::depot;
                int index2 = vehicleRoutes[i][0];
                currentDistance += vrp::distanceMatrix[index1][index2];
                // From the first node to the next to last
                for (size_t j = 0; j < vehicleRoutes[i].size() - 1; j++) {
                    index1 = vehicleRoutes[i][j];
                    index2 = vehicleRoutes[i][j + 1];
                    currentDistance += vrp::distanceMatrix[index1][index2];
                }
                // From last to depot
                index1 = vehicleRoutes[i][vehicleRoutes[i].size() - 1];
                index2 = vrp::depot;
                currentDistance += vrp::distanceMatrix[index1][index2];
                maxDistance = std::max(maxDistance, currentDistance);
            }
        }

        this->fitness = maxDistance;
    }
};

class VehicleRoutingProblem : public Problem<encoding>
{
public:
    VehicleRoutingProblem(const int dimension, const int _depot, const int _numberVehicle, const std::vector<std::vector<double>> _distanceMatrix,
                  OptimizationStrategy strategy, RepresentationType repType)
        : Problem(dimension, strategy, repType)
    {
        if (dimension == 0) {
            std::cerr << "Zero nodes were given as input\n";
            exit(1);
        }

        // Dimension * 2 since we have half of the decision variables to denote the permutation of the nodes,
        // and another half to denote to each node, the assigned vehicle. Check construct func. on the solution class
        this->dimension = dimension * 2;
        this->lb = std::vector<encoding>(this->dimension);
        this->ub = std::vector<encoding>(this->dimension);

        for (size_t i = 0; i < this->dimension; i++) {
            lb[i] = 0;
            ub[i] = nextafter(1.0, 0.0);
        }

        vrp::depot = _depot;
        vrp::numberVehicles = _numberVehicle;
        vrp::distanceMatrix = _distanceMatrix;
    }

    std::shared_ptr<Solution<encoding>> construct(std::vector<encoding> &decisionVariables) override
    {
        std::shared_ptr<VRPSolution> solution = std::make_shared<VRPSolution>(this->dimension, decisionVariables);
        // solution->localSearch();
        numbTriedSolution++;
        return solution;
    }
};

#endif // VRP_H
