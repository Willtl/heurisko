# Heurisko - C++ open-source parallel software for continuous and combinatorial optimization with metaheuristics
The project uses qmake, which, similarly to cmake, is responsible for generating platform-dependent makefiles. However, the project can be easily adapted to other control the software compilation process like cmake, and other IDEs. Note that Qt library <b>is not</b> a dependency.
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
#### The following strategies are associated with each populatiob-based metaheuristics:
- Opposition-based learning

### List of implemented single-solution metaheuristics:
Note that the methods below require that the `void localSearch()` function had been overridden in the solution class.
- Iterated local search
   - Random perturbation
   - Swap perturbation
   - 2-opt perturbation

### Problems addressed with the framework in the examples folder:
- Continuous optimization, non-convex functions
   - Rastrigin (indirect representation)
   - Ackley    (direct   representation)
- Combinatorial optimization
   - Traveling  salesman problem
      - 2-opt-based local search
      - swap-based local search
   - Vehicle routing problem
      - two decoding strategies
      - local search under development
   
