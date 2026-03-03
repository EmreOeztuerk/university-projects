/**
 * @file dispatcher.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Definition der Dispatcher-Klasse für die Nachrichtenverteilung.
 *
 * Der Dispatcher fungiert als zentrale Kommunikationskomponente, die Nachrichten
 * zwischen HAL, FSM und anderen Komponenten (auch maschinenübergreifend) weiterleitet.
 */

#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <iostream>
#include <sys/dispatch.h>
#include <sys/uio.h>
#include "IDispatcher.hpp"
#include "Verbindung.hpp"
#include "timer.hpp"
#include <unistd.h>
#include "../Dispatcher/signal.hpp"

// Name des Dispatchers
#define ATTACH_POINT1 "Dispatcher_1"
#define ATTACH_POINT2 "Dispatcher_2"

#define FEHLER -1

// Prioritaeten
#define EXTREM_HOCH 30
#define SEHR_HOCH_PRIORITAET 25
#define HOCH_PRIORITAET 20
#define MITTELHOCH_PRIORITAET 15
#define MITTEL_PRIORITAET 10
#define NORMAL_PRIORITAET 5

class Dispatcher : public IDispatcher {
private:
    name_attach_t *attachDIS;; // GNS
    int meinID;

    //------HAL---------
    int halID; // Channel ID der HAL

    //------FSMs---------
    int rootFSM; // Channel-ID des Betriebzustand


    //------Anlagen---------
    Verbindung *verbindung;
    Timer *heartbeatTimer;

public:
    Dispatcher(int id, std::string nachbarHost);

    virtual ~Dispatcher();

    int idd; // Anlage 1 oder Anlage 2
    /*
       * Liefert die Channel-ID des Dispatcher
       */
    int getKanal();

    // Superloop für die Weiterletung von Events
    void run();

    /*
       * Prioritaet zuweisen
       */
    int bestimmtPrioritaet(int code);

    /*
       * Dispatcher leiet eine Nachricht weiter.
       * coid: Ziel
       * code: typ von Signal
       * wert:z.B Groeße des WS
       */
    void weiterleiten(int coid, int code, int wert);

    /* Hilfsmethode für weiterleiten. Handlung von Signale und Befehle
       * code: typ von Signal
       * wert: z.B Groeße des WS
       */
    void signalHandlung(int code, int wert);


    /*Schnittstelle des Dispatcher für Logik und HAL, um Nachrichten zu senden
       * Signal: typ von Signal
       * Wert: z.B Groeße des WS
       */
    void sendenNachricht(Signal signal, int wert = 0);

    /*Schnittstelle des Dispatcher für Logik und HAL, um Nachrichten zu enthalten
       * Signal: typ von Signal
       * Wert: z.B Groeße des WS
       */
    bool nachrichtEmpfaengen(int chid, Signal &signal, int &wert);

    void restartHeartbeatTimer();

    bool istMeinSignal(int code);
};

#endif //DISPATCHER_H