# Heurisko - C++ open-source parallel framework for continuous and combinatorial optimization with metaheuristics

### Usage with GCC
Lines `[15, 16]` on file `heurisko.pro` should be commented.
```  
sudo apt update
sudo apt upgrade
sudo apt install g++ libomp-dev qt5-qmake
git clone https://github.com/Willtl/heurisko.git
qmake -makefile heurisko.pro
make
./heurisko
```
Alternatively, you can download and install Qt [IDE and libraries](https://www.qt.io/download). Clone the repository and open the project's folder on the QtCreator IDE.

### Usage with MSVC
Lines `[9, 12]` on file `heurisko.pro` should be commented.
Download and install Qt [IDE and libraries](https://www.qt.io/download), clone the repository and open the project's folder on the QtCreator IDE.

## List of implemented population-based metaheuristics:
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

## List of implemented single-solution metaheuristics:
The methods below require that the `void localSearch()` function had been overridden in the solution class.
- Iterated local search
   - Random perturbation
   - Swap perturbation
   - 2-opt perturbation

## Problems addressed with the framework in the examples folder:
- Non-convex continuous optimization examples
   - Rastrigin (indirect representation)
   - Ackley    (direct   representation)
- Combinatorial optimization
   - Traveling  salesman problem
      - 2-opt-based local search
      - swap-based local search
   - Vehicle routing problem
      - two decoding strategies 
   
