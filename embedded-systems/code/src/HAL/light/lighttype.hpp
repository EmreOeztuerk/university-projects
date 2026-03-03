/**
 * @file lighttype.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Definition der Enumeration TypLicht.
 *
 * Definiert die Bitmasken für die Zusatzleuchten Q1 und Q2 am Bedienpanel.
 */

#ifndef LIGHTTYPE_HPP
#define LIGHTTYPE_HPP

enum TypLicht {
	LED_Q1 = 1 << 4,
	LED_Q2 = 1 << 5
};

#endif
