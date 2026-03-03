/**
 * Trigger und Aktionen des Zustands "AnfangKalibrierung".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "anfang_kalibrierung.h"
#include "../motor_laeuft/motor_laeuft.h"
#include "../motor_laeuft/motor_laeuft2.h"

void AnfangKalibrierung::entry() {
    action->enteredAnfangKalibrierung();
    action->cmd_mtr_stop();
}

TriggerProcessingState AnfangKalibrierung::s_lsa1_ub() {
    cout << "Service Mode BCKalibrierung AnfangKalibrierung: lsa1_ub" << endl;

    if (data->festo1) {
        exitPointDefaultLeaveState();
        new(this) MotorLaeuft;
        entryPointDefaultEnter();
        return TriggerProcessingState::consumed;
    }
    return TriggerProcessingState::pending;
}

TriggerProcessingState AnfangKalibrierung::s_lse1_ub() {
    cout << "Service Mode BCKalibrierung AnfangKalibrierung: lse1_ub" << endl;

    if (data->festo2) {
        exitPointDefaultLeaveState();
        new(this) MotorLaeuft2;
        entryPointDefaultEnter();
        return TriggerProcessingState::consumed;
    }
    return TriggerProcessingState::pending;
}