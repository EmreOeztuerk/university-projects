/**
 * @file IDispatcher.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Interface für den Dispatcher.
 *
 * Definiert die abstrakte Schnittstelle, die von der HAL und anderen Komponenten
 * verwendet wird, um Nachrichten zu senden und zu empfangen, ohne die konkrete
 * Implementierung des Dispatchers kennen zu müssen.
 */

#ifndef IDISPATCHER_HPP
#define IDISPATCHER_HPP

#include "signal.hpp"

class IDispatcher {
public:
    virtual ~IDispatcher() = default;

    virtual void sendenNachricht(Signal signal, int wert = 0) = 0;

    virtual bool nachrichtEmpfaengen(int chid, Signal &signal, int &wert) = 0;
};

#endif