/**
 * Trigger und Aktionen des Zustands "MotorLaeuft3".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "motor_laeuft3.h"
#include "../werkstueck_laeuft/werkstueck_laeuft4.h"

void MotorLaeuft3::entry() {
    action->enteredMotorLaeuft3();
    action->cmd_mtr_rechts_s();
}

TriggerProcessingState MotorLaeuft3::s_lsa1_nub() {
    cout << "Service Mode BCKalibrierung MotorLaeuft3: lsa1_nub" << endl;

    exitPointDefaultLeaveState();
    new(this) WerkstueckLaeuft4;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}