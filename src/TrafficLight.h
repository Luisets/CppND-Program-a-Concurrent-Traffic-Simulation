#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"
#include "Utils.h"

// forward declarations to avoid include cycle
class Vehicle;

template <class T>
class MessageQueue
{
public:

    T receive();
    void send(T &&msg);
private:

    std::deque<T> _queue;
    std::mutex _mutex;
    std::condition_variable _condition;
};

enum class TrafficLightPhase : char
{
    red = 0,
    green
};

class TrafficLight : public TrafficObject
{
public:
    // constructor / desctructor
    TrafficLight();

    // getters / setters

    // typical behaviour methods
    void waitForGreen();
    void simulate();
    TrafficLightPhase getCurrentPhase();
private:
    // typical behaviour methods
    void cycleThroughPhases();

    TrafficLightPhase _currentPhase;
    MessageQueue<TrafficLightPhase> _trafficLightQueue;
    std::condition_variable _condition;
    std::mutex _mutex;
};

#endif