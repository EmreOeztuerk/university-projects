/**
 * @file buttontype.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Definition der Enumeration TypTaste.
 *
 * Definiert die Bitmasken für die Taster (Start, Stopp, Reset, E-Stop)
 * basierend auf ihrer GPIO-Pin-Belegung.
 */

#ifndef BUTTONTYPE_HPP
#define BUTTONTYPE_HPP

enum TypTaste {
	TASTE_START = 1 << 22,
	TASTE_STOP = 1 << 23,
	TASTE_RESET = 1 << 26,
	E_STOP = 1 << 27
};

#endif
