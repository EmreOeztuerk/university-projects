/**
 * @file timer.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Definition der Timer-Klasse.
 *
 * Diese Klasse kapselt QNX POSIX-Timer, um periodische oder einmalige
 * Pulse-Events an den Dispatcher zu senden.
 */

#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <sys/neutrino.h>
#include <time.h>

class Timer {
private:
    int coid; // Verbindung mit Dispatcher
    timer_t timerID; // ID vom Timer
    struct sigevent event; //Pulse Event

public:
    // chid: ID des Kanal des Dispatchers
    // code: Signal
    // wert: Daten
    Timer(int chid, int code, int value = 0);

    virtual ~Timer();

    // Timer starten in ms
    //nichtWiederholen: Timer wird nur einmal durchgefuehrt.
    void starten(int duration_ms, bool nichtWiederholen = true);

    // Timer stoppen, bevor die Zeit vergeht.
    void stoppen();
};


#endif /* TIMER_HPP_ */