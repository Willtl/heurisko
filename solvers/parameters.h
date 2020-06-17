#ifndef PARAMETERS_H
#define PARAMETERS_H

struct DifferentialEvolutionParameters {
    int numbIndividuals;	     // number of solutions generated per solution
    float crossoverRate;	     // value in [0,1]. Seems like that for high-dimensional problems, 0.0 leads to better performance
    float differentialWeight;    // value in [0,2]. Controls the amplification of the difference
    double localSearchAfterTime; // amount of time that the method must wait without an improvement to start doing local search
    bool oppositeLearning;	     // if opposite-based learning must be used or not
};

#endif // PARAMETERS_H
