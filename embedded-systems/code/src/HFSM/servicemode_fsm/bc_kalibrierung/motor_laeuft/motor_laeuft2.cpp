/**
 * Trigger und Aktionen des Zustands "MotorLaeuft2".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "motor_laeuft2.h"
#include "../werkstueck_laeuft/werkstueck_laeuft2.h"

void MotorLaeuft2::entry() {
    action->enteredMotorLaeuft2();
    action->cmd_mtr_rechts_l();
    data->zeitStarten();
}

TriggerProcessingState MotorLaeuft2::s_lsa2_ub() {
    cout << "Service Mode BCKalibrierung MotorLaeuft2: lsa2_nub" << endl;

    exitPointDefaultLeaveState();
    new(this) WerkstueckLaeuft2;
    entryPointDefaultEnter();
    return TriggerProcessingState::pending;
}