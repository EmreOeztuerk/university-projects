/**
 * Trigger und Aktionen des Zustands "MotorLaeuft4".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "motor_laeuft4.h"
#include "../werkstueck_laeuft/werkstueck_laeuft5.h"

void MotorLaeuft4::entry() {
    action->enteredMotorLaeuft4();
    action->cmd_mtr_rechts_s();
    data->zeitStarten();
}

TriggerProcessingState MotorLaeuft4::s_lsa2_ub() {
    cout << "Service Mode BCKalibrierung MotorLaeuft4: lsa2_nub" << endl;

    exitPointDefaultLeaveState();
    new(this) WerkstueckLaeuft5;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}