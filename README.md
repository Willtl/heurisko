# Heurisko - A C++ open-source parallel metaheuristic framework for continuous and combinatorial optimization
The project uses qmake, which, similarly to cmake, is responsible for generates platform-dependent makefiles. However, the project can be easily adapted to other control the software compilation process like cmake, and other IDEs.
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
      - Random mutation
      - Swap Mutation
      - Polynomial Mutation
      - Uniform Mutation 
- Particle Swarm Optimization
- Grey Wolf Optimizer
- Whale Optimization Algorithm
### List of implemented single-solution metaheuristics:
Note that the methods below require that the `void localSearch()` function had been overridden in the solution class.
- Iterated local search
   - Random perturbation
   - Swap perturbation
   - 2-opt perturbation

### Problems addressed with the framework in the examples folder:
- Continuous optimization, non-convex functions
   - Rastrigin
   - Ackley
- Combinatorial optimization
   - Traveling  salesman problem
      - 2-opt-based local search
      - swap-based local search
