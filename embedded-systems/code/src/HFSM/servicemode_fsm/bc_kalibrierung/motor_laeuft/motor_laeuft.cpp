/**
 * Trigger und Aktionen des Zustands "MotorLaeuft".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "motor_laeuft.h"
#include "../werkstueck_laeuft/werkstueck_laeuft.h"

void MotorLaeuft::entry() {
    action->enteredMotorLaeuft();
    action->cmd_mtr_rechts_l();
}

TriggerProcessingState MotorLaeuft::s_lsa1_nub() {
    cout << "Service Mode BCKalibrierung MotorLaeuft: lsa1_nub" << endl;

    exitPointDefaultLeaveState();
    new(this) WerkstueckLaeuft;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}