/**
 * Trigger und Aktionen des Zustands "AnfangKalibrierungSchnell".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 12. December 2025
 */
#include "anfang_kalibrierung_schnell.h"
#include "../motor_laeuft/motor_laeuft3.h"
#include "../motor_laeuft/motor_laeuft4.h"

void AnfangKalibrierungSchnell::entry() {
    action->enteredAnfangKalibrierungSchnell();
    action->cmd_mtr_stop();
}

TriggerProcessingState AnfangKalibrierungSchnell::s_lsa1_ub() {
    cout << "Service Mode BCKalibrierung AnfangKalibrierungSchnell: lsa1_ub" << endl;
    if (data->festo2 && !data->kalibrierungFertig) {
        new(this) MotorLaeuft4;
        entryPointDefaultEnter();
        return TriggerProcessingState::consumed;
    } else if (data->festo1) {
        new(this) MotorLaeuft3;
        entryPointDefaultEnter();
        return TriggerProcessingState::consumed;
    }
    return TriggerProcessingState::pending;
}