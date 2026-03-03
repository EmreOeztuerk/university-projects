/**
 * @file height.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Definition der Klasse Hoehenmesser (Sensor-Teil).
 *
 * Diese Klasse behandelt den digitalen Teil des Höhenmessers (die Lichtschranke dort).
 * Die analoge Messung erfolgt separat über ADC-Klassen.
 */

#ifndef HEIGHT_HPP
#define HEIGHT_HPP

#include "sensor.hpp"

class Hoehenmesser : public Sensor {
private:
    SensorTyp sensorTyp;
    Zustand sensorZustand;

public:
    Hoehenmesser();

    Zustand getZustand();

    SensorTyp getSensorTyp();
};

#endif