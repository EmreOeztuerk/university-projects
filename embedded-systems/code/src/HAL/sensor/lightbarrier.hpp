/**
 * @file lightbarrier.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Definition der Klasse Lichtschranke.
 *
 * Spezialisierung der Sensor-Klasse für Lichtschranken.
 */

#ifndef LIGHTBARRIER_HPP
#define LIGHTBARRIER_HPP

#include "sensor.hpp"

class Lichtschranke : public Sensor {
private:
    SensorTyp sensorTyp;
    Zustand sensorZustand;

public:
    Lichtschranke(SensorTyp given_sensorTyp);

    Zustand getZustand();

    SensorTyp getSensorTyp();

    void init_ISR_LB();

    void receivingRoutine_LB(int channelID);

    void handleInterrupt_LB();
};

#endif