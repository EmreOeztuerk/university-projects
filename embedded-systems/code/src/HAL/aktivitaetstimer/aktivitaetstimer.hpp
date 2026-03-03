/**
 * @file aktivitaetstimer.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Definition der Klasse AktivitaetsTimer.
 *
 * Dient zur Messung von Zeitspannen, z.B. wie lange ein Taster gedrückt wurde
 * (um zwischen kurz und lang drücken zu unterscheiden).
 */
#include <sys/neutrino.h>
#include <sys/syspage.h>
#include <inttypes.h>

#ifndef AKTIVITAETSTIMER_HPP
#define AKTIVITAETSTIMER_HPP

class AktivitaetsTimer {
private:
	uint64_t start;
	double sec;

public:
	AktivitaetsTimer();
	void zeitStarten();
	void zeitSpeichern(int &temp);
	//
};

#endif
