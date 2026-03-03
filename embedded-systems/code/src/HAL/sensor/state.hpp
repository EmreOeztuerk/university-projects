/**
 * @file state.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Definition der Enumeration Zustand.
 *
 * Beschreibt den physikalischen Zustand einer Lichtschranke (unterbrochen/frei).
 */

#ifndef STATE_HPP
#define STATE_HPP

enum Zustand {
	NICHT_UNTERBROCHEN,
	UNTERBROCHEN,
	HOEHE_OK
};

#endif
