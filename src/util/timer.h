#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <iostream>
#include <memory>

class Timer
{

public:
    Timer() { startTimePoint = std::chrono::high_resolution_clock::now(); }

    ~Timer() { Stop(); }
    void Stop()
    {
        std::chrono::time_point<std::chrono::high_resolution_clock> endTimePoint = std::chrono::high_resolution_clock::now();

        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(startTimePoint).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();
        // Microseconds
        auto duration = end - start;
        double ms = duration * 0.001;
        double sec = ms * 0.001;

        std::cout << "microsec. " << duration << ", millisec. " << ms << ", sec. " << sec << "\n";
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTimePoint;
};

#endif // TIMER_H
