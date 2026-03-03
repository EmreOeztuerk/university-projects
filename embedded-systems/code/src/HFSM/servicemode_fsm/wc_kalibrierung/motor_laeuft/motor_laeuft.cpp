/**
 * Trigger und Aktionen des Zustands "MotorLaeuft".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "motor_laeuft.h"

void MotorLaeuftWC::entry() {
    action->enteredMotorLaeuftWC();
    if (data->letztesMal) {
        action->cmd_mtr_rechts_s();
    } else {
        action->cmd_mtr_rechts_l();
    }
}

TriggerProcessingState MotorLaeuftWC::s_lsa1_nub() {
    cout << "Service Mode WCKalibrierung MotorLaeuft: lsa1_nub" << endl;

    exitPointDefaultLeaveState();
    new(this) WerkstueckLaeuftWC;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}