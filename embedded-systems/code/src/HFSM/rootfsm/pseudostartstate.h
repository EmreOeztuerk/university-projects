/**
* @file pseudostartstate.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Initialer Pseudo-Zustand der Root-FSM.
 *
 * Dient als Einstiegspunkt beim Starten der HFSM und transitiert sofort
 * in den ersten echten Zustand (Ruhezustand).
 */

#ifndef PSEUDOSTARTSTATE_H
#define PSEUDOSTARTSTATE_H

#include "BaseState.h"

class PseudoStartState : public BaseState {
public:
    bool isPseudoStartState() override;

    void showState() override; // Just for illustration!
};

#endif //PSEUDOSTARTSTATE_H