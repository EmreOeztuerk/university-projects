/**
 * @file sensor.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Definition der Basisklasse Sensor.
 *
 * Verwaltet die Interrupt-Behandlung für GPIO-Sensoren (Lichtschranken, Metallsensor)
 * sowie die Kommunikation mit dem Dispatcher bei Statusänderungen.
 */

#ifndef SENSOR_HPP
#define SENSOR_HPP

#include "../../gpio_address.hpp"
#include "../hight/ADC.h"
#include "../../Dispatcher/timer.hpp"
#include "../../Dispatcher/signal.hpp"
#include "../../Dispatcher/IDispatcher.hpp"
#include "sensortype.hpp"
#include "state.hpp"

#define DEBOUCE_OFFSET 0x150
#define DEBOUCE_TIME_OFFSET 0x154
//#define PULSE_RUTSCHE_VOLL 106

class Sensor {
private:
    int idd;
    int letzesMal = -1;
    std::chrono::steady_clock::time_point lastTime;
    SensorTyp sensorTyp;
    Zustand sensorZustand;
    IDispatcher *dispatcher;
    int interruptID;
    bool receivingRunning;

    Timer *rutscheTimer = nullptr; // Timer für die Rutsche


    int chanID;
    int conID;

public:
    //Sensor (SensorTyp given_sensorTyp);
    Sensor(SensorTyp given_sensorTyp, IDispatcher *given_disp, int id);

    Zustand getZustand();

    SensorTyp getSensorTyp();

    void setZustand(Zustand t);

    void sensor_isr();

    void receivingRoutine(int channelID, ADC *adc);

    int berechne(uintptr_t gpioBase, int intrStatusReg);

    void tempUB();

    void tempNUB();
};

#endif