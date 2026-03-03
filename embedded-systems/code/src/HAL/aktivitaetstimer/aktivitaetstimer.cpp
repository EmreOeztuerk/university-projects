/**
 * @file aktivitaetstimer.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung des AktivitaetsTimers.
 */

#include "aktivitaetstimer.hpp"

AktivitaetsTimer::AktivitaetsTimer() {
	start = 0;
	sec = 0;
}

void AktivitaetsTimer::zeitStarten() {
	start = ClockCycles();
}

void AktivitaetsTimer::zeitSpeichern(int &temp) {
	uint64_t finish = ClockCycles();
	uint64_t diff = finish - start;
	uint64_t cps = SYSPAGE_ENTRY(qtime)->cycles_per_sec;
	sec = (double)diff / cps;
	temp = (int) sec;
}
