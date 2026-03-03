/**
 * @file buttonlight.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Definition der Enumeration TasteLicht.
 *
 * Definiert die Bitmasken für die LEDs, die in die Taster integriert sind (Start-LED, Reset-LED).
 */

#ifndef BUTTONLIGHT_HPP
#define BUTTONLIGHT_HPP

enum TasteLicht {
	LED_START = 1 << 2,
	LED_RESET = 1 << 3,
	KEIN_LED = 0
};

#endif
