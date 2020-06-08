#ifndef SOLUTION_H
#define SOLUTION_H

#include <memory>

template <class T>
class Solution
{
public:
    double getFitness() const { return fitness; }
    void setFitness(const double &value) { fitness = value; }
    std::vector<T> &getDecisionVariables() { return decisionVariables; }
    void setDecisionVariables(const std::vector<T> &value) { decisionVariables = value; }

    virtual void print()
    {
        for (size_t i = 0; i < decisionVariables.size(); i++) {
            std::cout << decisionVariables[i] << ", ";
        }
        std::cout << std::endl;
    }

    virtual void localSearch()
    {
        std::cerr << "You must override this function in your derived class" << std::endl;
        exit(EXIT_FAILURE);
    }

protected:
    Solution(std::vector<T> &decisionVariables) : decisionVariables(decisionVariables) {}
    double fitness;
    std::vector<T> &decisionVariables;
};
#endif // SOLUTION_H
