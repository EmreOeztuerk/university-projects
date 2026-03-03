/**
 * @file color.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Definition der Enumeration Farbe.
 *
 * Definiert die Bitmasken für die Ansteuerung der Ampel-LEDs am GPIO-Port.
 */

#ifndef COLOR_HPP
#define COLOR_HPP

/**
 * Enumeration zur Definition der Farben der Ampel.
 */
enum Farbe {
	LED_ROT = 1 << 16,
	LED_GELB = 1 << 17,
	LED_GRUEN = 1 << 18
};

#endif
