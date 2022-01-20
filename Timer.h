#include <Arduino.h>

class Timer
{
private:
    bool isPeriodical = false;
    bool isAlreadyTriggered = false;
    unsigned long int trigTime = 0;
    unsigned int period = 0;
public:
    Timer(bool isPeriodical, unsigned int period) {
        this->isPeriodical = isPeriodical;
        this->period = period;
    }
    Timer() {

    }
    ~Timer() {

    }

    void setTimer(long int newTime) {
        this->trigTime = newTime;
        isAlreadyTriggered = false;
    }
    

    bool checkTimer() {
        if(millis() >= trigTime && !isAlreadyTriggered) {
            isAlreadyTriggered = true;
            if(isPeriodical)  {
                setTimer(millis() + period);
            }            
            return true;
        }
        else return false;
    }
};

