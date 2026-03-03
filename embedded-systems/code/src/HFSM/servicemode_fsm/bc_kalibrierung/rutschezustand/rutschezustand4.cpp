/**
 * Trigger und Aktionen des Zustands "Rutschezustand4".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "rutschezustand4.h"
#include "../motor_laeuft/motor_laeuft4.h"

void Rutschezustand4::entry() {
    action->enteredRutschezustand4();
    //data->zeitSpeichern(data->t_lsm_lsr, "");
    data->setSortTyp(SortTyp::WEICHE);
}

TriggerProcessingState Rutschezustand4::s_lsr2_nub() {
    cout << "Service Mode BCKalibrierung Rutschezustand4: lsr2_nub" << endl;

    if (data->sortTyp == SortTyp::WEICHE) {
        exitPointDefaultLeaveState();
        new(this) MotorLaeuft4;
        entryPointDefaultEnter();
        return TriggerProcessingState::consumed;
    }
    return TriggerProcessingState::pending;
}