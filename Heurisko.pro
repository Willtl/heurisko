QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CXXFLAGS -= -O1
QMAKE_CXXFLAGS -= -O2
QMAKE_CXXFLAGS += -O3 -fopenmp -pthread
QMAKE_LFLAGS += -fopenmp

SOURCES += main.cpp
 
HEADERS += \
    entities/solution.h \
    entities/problem.h \
    examples/continuous/direct/functions.h \
    examples/continuous/indirect/rastrigin.h \
    examples/continuous/direct/ackley.h \
    examples/continuous/direct/crossintray.h \
    solvers/differentialevolution.h \
    solvers/globalsolver.h \
    solvers/geneticalgorithm.h \
    solvers/greywolfoptimizer.h \
    solvers/particleswarmoptimization.h \
    solvers/solver.h \
    util/util.h \
    util/tspreader.h \
    examples/combinatorial/indirect/tsp.h \
    solvers/iteratedlocalseach.h


