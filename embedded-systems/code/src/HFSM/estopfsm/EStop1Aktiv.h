/**
 * @file EStop1Aktiv.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Konkreter Zustand der EStop-Sub-State-Machine.
 *
 * EStop1Aktiv repräsentiert den Zustand, in dem
 * der EStop 1 (E1) aktiv ist.
 *
 * In diesem Zustand:
 *  - EStop 1 ist gedrückt
 *  - EStop 2 kann zusätzlich ausgelöst werden
 *  - EStop 1 kann wieder freigegeben werden
 */

#ifndef ESTOP1AKTIV_H
#define ESTOP1AKTIV_H

#include "EstopBaseState.h"

/**
 * @class EStop1Aktiv
 * @brief Zustand: E-Stop 1 aktiv.
 *
 * Behandelt alle Trigger, die relevant sind,
 * während der E-Stop 1 aktiv ist.
 */
class EStop1Aktiv : public EStopBaseState {
public:
    void entry() override;

    TriggerProcessingState s_e2_g() override;

    TriggerProcessingState s_e1_ng() override;

    void showState() override;
};

#endif /* ESTOP1AKTIV_H */