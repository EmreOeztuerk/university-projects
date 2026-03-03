/**
 * @file timer.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Definition der Klasse TempTimer zur Zeitmessung mittels CPU-Zyklen.
 *
 * Diese Klasse nutzt die QNX-Systemfunktion `ClockCycles()`, um präzise Zeitmessungen
 * durchzuführen (z.B. für Performance-Tests oder kurze Delays).
 */

#include <sys/neutrino.h>
#include <sys/syspage.h>
#include <inttypes.h>

#ifndef TIMER_HPP
#define TIMER_HPP

class TempTimer {
private:
	uint64_t start;
	double sec;

public:
	TempTimer();
	void zeitStarten();
	void zeitSpeichern(int &temp);
	//
};

#endif
