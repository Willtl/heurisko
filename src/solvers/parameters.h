#ifndef PARAMETERS_H
#define PARAMETERS_H

// Differential Evolution Parameters
struct DifferentialEvolutionParameters {
    int numbIndividuals;	     // number of solutions generated per solution
    float crossoverRate;	     // value in [0,1]. Seems like that for high-dimensional problems, 0.0 leads to better performance
    float differentialWeight;    // value in [0,2]. Controls the amplification of the difference
    double localSearchAfterTime; // amount of time that the method must wait without an improvement to start doing local search
    bool oppositeLearning;	     // if opposite-based learning must be used or not
};

// Genetic Algorithm Parameters
enum CrossoverType { UNIFORM, ONE_POINT, TWO_POINT, SIMULATED_BINARY };
enum SelectionType { FITNESS_PROPORTIONATE, TOURNAMENT };
enum MutationType { RANDOM_MUTATION, POLYNOMIAL, SWAP_MUTATION, SCRAMBLE_MUTATION };

struct GeneticAlgorithmParameters {
    int numbIndividuals; // number of solutions generated per solution
    float crossoverRate; // value in [0,1]
    float mutationRate;  // value in [0,1]
    CrossoverType crossoverType;
    SelectionType selectionType;
    MutationType mutationType;
};

#endif // PARAMETERS_H
