#pragma once

#include <chrono>
#include <random>

// Return a chrono time between the given interval.
// Pre: T should be a str::chrono type
template <typename T>
T getChonoBetweenInterval(int downLimit, int upLimit)
{
    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<> distr(downLimit, upLimit);
    return T(distr(eng));
}