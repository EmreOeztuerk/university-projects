#include "../../gpio_address.hpp"
#include "../hight/ADC.h"
#include "../../Dispatcher/timer.hpp"
#include "../../Dispatcher/signal.hpp"
#include "../../Dispatcher/IDispatcher.hpp"
#include "sensortype.hpp"
#include "state.hpp"

#ifndef SENSOR2_HPP
#define SENSOR2_HPP

class Sensor2 {
private:
    SensorTyp sensorTyp;
    Zustand sensorZustand;
    int interruptID;
    bool receivingRunning;

    int chanID;
    int conID;

public:
    Sensor2();

    Sensor2(SensorTyp given_sensorTyp);

    virtual ~Sensor2() {
    };

    void temp_isr();

    void receivingRoutine(int channelID);

    void setSensor(SensorTyp giv);

    int init_sens();

    Zustand getZustand();

    void setZustand(Zustand t);

    //	int berechne(uintptr_t gpioBase, SensorTyp x);
    int berechne(uintptr_t gpioBase, int x);
};

#endif