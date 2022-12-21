#include <iostream>
#include <random>
#include "TrafficLight.h"
#include "Utils.h"

/* Implementation of class "MessageQueue" */

template <typename T>
T MessageQueue<T>::receive()
{
    auto lock = std::unique_lock<std::mutex>(_mutex);
    _condition.wait(lock, [&]()
                    { return !_queue.empty(); });

    auto &msg = _queue.front();
    _queue.pop_front();
    return std::move(msg);
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _queue.clear();
    _queue.emplace_back(msg);
    _condition.notify_one();
}

/* Implementation of class "TrafficLight" */
TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    while(_trafficLightQueue.receive() != TrafficLightPhase::green);
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    TrafficObject::threads.emplace_back(std::move(std::thread(&TrafficLight::cycleThroughPhases, this)));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    std::chrono::time_point<std::chrono::system_clock> lastUpdate;
    long cycleDuration = getChonoBetweenInterval<std::chrono::milliseconds>(4000, 6000).count(); // run cycle in ms
    lastUpdate = std::chrono::system_clock::now();
    while (true)
    {
        // wait to reduce CPU ussage
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        // calculate elapsed time since last run
        long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastUpdate).count();

        if (timeSinceLastUpdate < cycleDuration)
        {
            continue;
        }

        // toggle _currentPhase
        _currentPhase = (_currentPhase == TrafficLightPhase::red) ? TrafficLightPhase::green : TrafficLightPhase::red;
        
        auto tempPhase = _currentPhase;
        _trafficLightQueue.send(std::move(tempPhase));

        // reset for new run cycle
        lastUpdate = std::chrono::system_clock::now();
    }
}