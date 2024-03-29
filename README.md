# Heurisko

A C++ platform to perform parallel computations of optimization tasks via metaheuristics. 

```  
sudo apt update && sudo apt install g++ libomp-dev cmake
git clone https://github.com/Willtl/heurisko.git
cd heurisko
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release 
make
./heurisko
```

CMakeLists.txt is configured for the `travellingsalesmanproblem.cpp` example. For different examples, change accordingly before building.

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

#### The following strategies are associated with each population-based metaheuristics:
- Opposition-based learning

### List of implemented single-solution metaheuristics:
The methods below require that the `void localSearch()` function had been overridden in the solution class.
- Iterated local search
   - Random perturbation
   - Swap perturbation
   - 2-opt perturbation

### Problems addressed with the framework in the examples folder:
- Non-convex continuous optimization examples
   - Rastrigin (indirect representation)
   - Ackley    (direct   representation)
- Combinatorial optimization
   - Traveling  salesman problem
      - 2-opt-based local search
      - swap-based local search
   - Vehicle routing problem
      - two decoding strategies 
   
