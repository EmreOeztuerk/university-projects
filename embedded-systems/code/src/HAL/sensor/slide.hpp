/**
 * @file slide.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Definition der Enumeration RutscheZustand.
 *
 * Beschreibt den Füllstand der Rutsche.
 */

#ifndef SLIDE_HPP
#define SLIDE_HPP

#include "sensor.hpp"

class Rutsche : public Sensor {
private:
	SensorTyp sensorTyp;
	Zustand sensorZustand;

public:
	Rutsche();
	bool istVoll();
	Zustand getZustand();
	SensorTyp getSensorTyp();
};

#endif
