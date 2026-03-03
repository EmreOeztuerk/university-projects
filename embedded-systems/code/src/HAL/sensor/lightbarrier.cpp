/**
* @file lightbarrier.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung der Lichtschranke-Klasse.
 */

#include "lightbarrier.hpp"

Zustand Lichtschranke::getZustand() {
    return sensorZustand;
}

SensorTyp Lichtschranke::getSensorTyp() {
    return sensorTyp;
}

void Lichtschranke::init_ISR_LB() {
    //
}

void Lichtschranke::receivingRoutine_LB(int channelID) {
    //
}

void Lichtschranke::handleInterrupt_LB() {
    //
}