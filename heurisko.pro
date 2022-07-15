QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS

# The flags below must be activated if you are using GCC compiler
QMAKE_CXXFLAGS -= -O1
QMAKE_CXXFLAGS -= -O2
QMAKE_CXXFLAGS += -O3 -fopenmp
QMAKE_LFLAGS += -fopenmp

# The flags below must be activated if you are using MSVC compiler
# QMAKE_CXXFLAGS += /O2 /openmp
# QMAKE_LFLAGS += /openmp

SOURCES += examples/travellingsalesmanproblem.cpp
 
HEADERS += \
    src/solution.h \
    src/problem.h \
    examples/combinatorial/indirect/vrp.h \
    examples/continuous/direct/functions.h \
    examples/continuous/indirect/rastrigin.h \
    examples/continuous/direct/ackley.h \
    examples/continuous/direct/crossintray.h \
    src/solvers/differentialevolution.h \
    src/solvers/globalsolver.h \
    src/solvers/geneticalgorithm.h \
    src/solvers/greywolfoptimizer.h \
    src/solvers/parameters.h \
    src/solvers/particleswarmoptimization.h \
    src/solvers/solver.h \
    src/solvers/iteratedlocalseach.h \
	src/util/timer.h \
    src/util/util.h \
    src/util/tspreader.h \
    examples/combinatorial/indirect/tsp.h 
    
