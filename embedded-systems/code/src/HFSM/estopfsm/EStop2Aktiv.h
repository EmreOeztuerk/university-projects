/**
 * @file EStop2Aktiv.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Konkreter Zustand der EStop-Sub-State-Machine.
 *
 * EStop2Aktiv repräsentiert den Zustand, in dem
 * der Not-Aus 2 (E2) aktiv ist.
 *
 * In diesem Zustand:
 *  - EStop 2 ist gedrückt
 *  - EStop 1 kann zusätzlich ausgelöst werden
 *  - EStop 2 kann wieder freigegeben werden
 */

#ifndef ESTOP2AKTIV_H
#define ESTOP2AKTIV_H

#include "EstopBaseState.h"


/**
 * @class EStop1Aktiv
 * @brief Zustand: E-Stop 2 aktiv.
 *
 * Behandelt alle Trigger, die relevant sind,
 * während der EStop 2 aktiv ist.
 */
class EStop2Aktiv : public EStopBaseState {
public:
    void entry() override;

    TriggerProcessingState s_e2_ng() override;

    TriggerProcessingState s_e1_g() override;

    void showState() override;
};

#endif /* ESTOP2AKTIV_H */