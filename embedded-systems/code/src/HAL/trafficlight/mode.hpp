/**
 * @file mode.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Definition der Enumeration Modus.
 *
 * Beschreibt die verschiedenen Betriebszustände einer einzelnen Ampelleuchte.
 */

#ifndef MODE_HPP
#define MODE_HPP

/**
 * Enumeration zur Definition der Modi der einzelnen Ampel-Lichter.
 */
enum Modus {
	LED_AN,
	LED_AUS,
	// t (sec.) = 1 / Hz
	BLINK_LANGSAM = 2,
	BLINK_SCHNELL = 1
};

#endif
