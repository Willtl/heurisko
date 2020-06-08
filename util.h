#ifndef UTIL_H
#define UTIL_H

#include <chrono>
#include <iomanip>
#include <iostream>
#include <float.h>
#include <random>
#include <vector>

namespace utils
{
double maxRunningTime;

std::chrono::steady_clock::time_point start;
int getRandom(int rightBound)
{
    std::random_device rd;				       // only used once to initialise (seed) engine
    std::mt19937 rng(rd());				       // random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> uni(0, rightBound); // guaranteed unbiased
    return uni(rng);
}

double getRandom()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0., 1.);
    return dist(gen);
}

void startTimeCounter() { start = std::chrono::steady_clock::now(); }
double getCurrentTime() { return std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - start).count(); }

template <class T1, class T2>
T2 remap(T1 n, T1 start1, T1 stop1, T1 start2, T1 stop2)
{
    return ((n - start1) / (double)(stop1 - start1)) * (stop2 - start2) + start2;
}

template <typename T>
void printVector(const T &arrayValues)
{
    std::cout.precision(17);
    std::cout << "[";
    for (size_t i = 0; i < arrayValues.size(); i++) {
        if (i + 1 < arrayValues.size())
            std::cout << arrayValues[i] << ", ";
        else
            std::cout << arrayValues[i];
    }
    std::cout << "]" << std::endl;
    ;
}

template <typename T>
void printValueAndTime(const T value, const double time)
{
    std::cout << std::setw(17) << "Objective value: " << std::setw(35) << std::setprecision(35) << value << std::setw(35) << " time: " << std::setw(25) << time << std::endl;
}

// CONSTANTS
double NEXT_ONE_TO_ZERO = 0.99999999999999999999999999999999999999999999999999999999999999999999999;

} // namespace utils

#endif // UTIL_H
