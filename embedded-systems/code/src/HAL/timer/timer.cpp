/**
 * @file timer.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung der TempTimer-Klasse.
 *
 * Berechnet die verstrichene Zeit basierend auf der Differenz der `ClockCycles()`
 * und der System-Frequenz (`cycles_per_sec`).
 */

#include "timer.hpp"

TempTimer::TempTimer() {
	start = 0;
	sec = 0;
}

void TempTimer::zeitStarten() {
	start = ClockCycles();
}

void TempTimer::zeitSpeichern(int &temp) {
	uint64_t finish = ClockCycles();
	uint64_t diff = finish - start;
	uint64_t cps = SYSPAGE_ENTRY(qtime)->cycles_per_sec;
	sec = (double)diff / cps;
	temp = (int) sec;
}
