/**
 * @file timer.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung der Timer-Klasse.
 *
 * Verwaltet die Erstellung (`timer_create`), Konfiguration (`timer_settime`)
 * und Löschung der Timer sowie die Verbindung zum Dispatcher-Channel.
 */

#include "timer.hpp"
#include <iostream>
#include <sys/netmgr.h>
using namespace std;

Timer::Timer(int chid, int code, int value) {
    coid = ConnectAttach(ND_LOCAL_NODE, 0, chid, _NTO_SIDE_CHANNEL, 0);

    if (coid == -1) {
        perror("[Timer] ConnectAttach hat fehlgeschlagen");
    }

    SIGEV_PULSE_INIT(
        &event,
        coid,
        SIGEV_PULSE_PRIO_INHERIT,
        code, // Signal
        value // Daten
    );

    if (timer_create(CLOCK_REALTIME, &event, &timerID) == -1) {
        perror("[Timer] Timer creation failed");
    }
}

Timer::~Timer() {
    stoppen();
    timer_delete(timerID);
    ConnectDetach(coid);
}

void Timer::starten(int duration_ms, bool periodic) {
    struct itimerspec timer_spec;

    // Sekunden und Nanosekunden
    long seconds = duration_ms / 1000;
    long nanoseconds = (duration_ms % 1000) * 1000000;


    timer_spec.it_value.tv_sec = seconds;
    timer_spec.it_value.tv_nsec = nanoseconds;

    // Wiederholung des Timers
    if (periodic) {
        timer_spec.it_interval.tv_sec = seconds;
        timer_spec.it_interval.tv_nsec = nanoseconds;
    } else {
        timer_spec.it_interval.tv_sec = 0;
        timer_spec.it_interval.tv_nsec = 0;
    }


    if (timer_settime(timerID, 0, &timer_spec, NULL) == -1) {
        perror("[Timer] Setting Timer hat fehlgeschlagen.");
    } else {
        cout << "[Timer] Startet für " << duration_ms << "ms" << endl;
    }
}

void Timer::stoppen() {
    struct itimerspec timer_spec;
    //Setzt alles auf 0
    timer_spec.it_value.tv_sec = 0;
    timer_spec.it_value.tv_nsec = 0;
    timer_spec.it_interval.tv_sec = 0;
    timer_spec.it_interval.tv_nsec = 0;

    timer_settime(timerID, 0, &timer_spec, NULL);
}