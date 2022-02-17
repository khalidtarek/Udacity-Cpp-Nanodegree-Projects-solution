#include <stdio.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */
 
template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 

    // mutex unique lock
    std::unique_lock<std::mutex> lockMutex(_mutex);

    // pass unique lock to condition variable
    _condition.wait(lockMutex, [this] { return !_trafficLightMessageQueue.empty(); });

    // remove last vector element from queue
    T trafficLightMessage = std::move(_trafficLightMessageQueue.back());
    _trafficLightMessageQueue.pop_back();

    // will not be copied due to rvo
    return trafficLightMessage;
}

template <typename T>
void MessageQueue<T>::send(T &&trafficLightMessage)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.

    // mutex lock
    std::lock_guard<std::mutex> lockMutex(_mutex);

    // add message to queue 
    _trafficLightMessageQueue.push_back(std::move(trafficLightMessage));

    // notify client after pushing new message into vector 
    _condition.notify_one(); 
}

/* Implementation of class "TrafficLight" */

TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.

    while(true)
    {
        if(_trafficLightMessageQueue.receive() == green)
        {
          	break;
        }
        else
        {
           continue;
        }
    }
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class. 
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles 
    // and toggles the current phase of the traffic light between red and green and sends an update method 
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. 
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles. 

    // variable for random variable generated value
    int randomCycleDuration;
    // variable for time calculation start value
    std::chrono::time_point<std::chrono::system_clock> calculationStartTime;
    // variable for acual duration
    long actualCycleDuration;

    // initialize random seed
    srand (time(NULL));

    // generate secret number between 4000 and 6000
    randomCycleDuration = 4000 + rand() % 2001;
    
    // Get current system clock for time calculation
    calculationStartTime = std::chrono::system_clock::now();

    while(true)
    {
        // Calculate difference between last stored time and current system time 
        actualCycleDuration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - calculationStartTime).count();

        // Check if time exceeded random generated number
        if(actualCycleDuration >= randomCycleDuration)
        {
            // Toggle traffic light between green and red
            _currentPhase = _currentPhase == red ? green : red;

            // send current traffic lite to queue using move semantics
            _trafficLightMessageQueue.send(std::move(_currentPhase));

            // Generate Random Number for the Next Cycle
            randomCycleDuration = 4000 + (rand() % 2001);

            // Get current system clock for next time calculation
            calculationStartTime = std::chrono::system_clock::now();
        }

        // sleep for 1ms between 2 cycles to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
