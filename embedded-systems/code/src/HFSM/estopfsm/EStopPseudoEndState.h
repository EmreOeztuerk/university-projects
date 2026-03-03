/**
 * @file EStopPseudoEndState.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Pseudo-End-Zustand der EStop-Sub-State-Machine.
 *
 * Dieser Zustand markiert das formale Ende einer
 * Sub-State-Machine und besitzt keine eigene Logik.
 * Er dient ausschließlich zur Strukturierung der FSM.
 */


#ifndef ESTOPPSEUDOENDSTATE_H
#define ESTOPPSEUDOENDSTATE_H

#include "estopbasestate.h"
#include <iostream>

/**
 * @class EStopPseudoEndState
 * @brief Pseudo-End-Zustand der EStop-FSM.
 *
 * Wird verwendet, um das Ende eines Sub-Zustands
 * explizit darzustellen (z. B. für Default-Exit-Übergänge).
 */
class EStopPseudoEndState : public EStopBaseState {
public:
    /**
     * @brief Kennzeichnet diesen Zustand als Pseudo-End-State.
     */
    bool isPseudoEndState() override { return true; };

    /**
     * @brief Ausgabe des Zustands (nur zu Debug-Zwecken).
     */
    void showState() override { std::cout << "State: EStop End State" << std::endl; }; // Just for illustration!
};

#endif /* ESTOPPSEUDOENDSTATE_H */