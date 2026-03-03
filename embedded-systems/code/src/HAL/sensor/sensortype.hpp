/**
 * @file sensortype.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Definition der Enumeration SensorTyp.
 *
 * Definiert Bitmasken für die Identifikation der verschiedenen Sensoren
 * im Interrupt-Status-Register.
 */

#ifndef SENSORTYPE_HPP
#define SENSORTYPE_HPP

enum SensorTyp {
    UNDEFINIERT = 0,
    LS_EINLAUF = 1 << 2,
    HOEHENMESSER = 1 << 3,
    METALL = 1 << 7,
    LS_SORTIERER = 1 << 5,
    LS_RUTSCHE = 1 << 15,
    LS_AUSLAUF = 1 << 20
};

#endif