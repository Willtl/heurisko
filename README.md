# Heurisko - C++ open source parallel metaheuristic framework

### List of implemented population-based metaheuristics:
- Differential Evolution
   - Mutation scheme variants:
      - DE/rand/1
      - DE/best/1
- Genetic Algorithm
   - Crossover operators
      - Uniform Crossover
      - Simulated Binary Crossover
      - Multiple Point Crossover 
   - Mutation operators
      - Polynomial Mutation
      - Uniform Mutation 
- Particle Swarm Optimization
- Grey Wolf Optimizer
- Whale Optimization Algorithm

### Problems addressed with the framework in the examples folder:
- Continuous optimization, non-convex functions
   - Rastrigin
   - Ackley
- Combinatorial optimization
   - Travelling salesman problem
      - 2-opt-based local search
      - swap-based local search
