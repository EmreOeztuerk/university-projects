/**
 * @file speed.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Definition der Enumeration Geschw.
 *
 * Definiert die Bitwerte für die Geschwindigkeitssteuerung des Motors
 * (Stopp, Langsam, Schnell).
 */

#ifndef SPEED_HPP
#define SPEED_HPP

/**
 * Enumeration zur Definition der Geschwindigkeiten des Motors.
 */
enum Geschw {
	MOTOR_SCHNELL = 0,
	MOTOR_LANGSAM = 1 << 14,
	MOTOR_STOP = 1 << 15
};

#endif
