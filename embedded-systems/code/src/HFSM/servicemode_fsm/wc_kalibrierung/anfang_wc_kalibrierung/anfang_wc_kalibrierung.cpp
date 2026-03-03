/**
 * Trigger und Aktionen des Zustands "AnfangWCKalibrierung".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "anfang_wc_kalibrierung.h"
#include "../motor_laeuft/motor_laeuft.h"

void AnfangWCKalibrierung::entry() {
    action->enteredAnfangWCKalibrierung();
    action->cmd_mtr_stop();
}

TriggerProcessingState AnfangWCKalibrierung::s_lsa1_ub() {
    cout << "Service Mode AnfangWCKalibrierung: lsa1_ub" << endl;

    exitPointDefaultLeaveState();
    new(this) MotorLaeuftWC;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}