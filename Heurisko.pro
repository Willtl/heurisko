QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QMAKE_CXXFLAGS -= -O1
QMAKE_CXXFLAGS -= -O2
QMAKE_CXXFLAGS += -O3 -fopenmp -pthread
QMAKE_LFLAGS += -fopenmp

SOURCES += main.cpp

INCLUDEPATH += "/home/willian/workspace/qt/SatelliteStructureOptimization/eigen/Eigen"

HEADERS += \
    util.h \
    differentialevolution.h \
    solver.h \
    globalsolver.h \
    problem.h \
    geneticalgorithm.h \
    greywolfoptimizer.h \
    solvers.h \
    functions.h \
    particleswarmoptimization.h \
    localsearch.h \
    solution.h
