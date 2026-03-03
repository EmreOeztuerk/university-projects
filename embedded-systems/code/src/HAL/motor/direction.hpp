/**
 * @file direction.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Definition der Enumeration Richtung.
 *
 * Definiert die Bitwerte für die Laufrichtung des Motors (Rechts, Links).
 */

#ifndef DIRECTION_HPP
#define DIRECTION_HPP

/**
 * Enumeration zur Definition der Richtungen in welche der Motor laufen kann.
 */
enum Richtung {
	MOTOR_RECHTS = 1 << 12,
	MOTOR_LINKS = 1 << 13,
	MOTOR_STEHEN = 0
};

#endif
