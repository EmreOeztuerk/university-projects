/**
 * Trigger und Aktionen des Zustands "Rutschezustand2".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "rutschezustand2.h"
#include "../motor_laeuft/motor_laeuft2.h"

void Rutschezustand2::entry() {
    action->enteredRutschezustand2();
    data->setSortTyp(SortTyp::WEICHE);
}

TriggerProcessingState Rutschezustand2::s_lsr2_nub() {
    cout << "Service Mode BCKalibrierung Rutschezustand2: lsr2_nub" << endl;

    if (data->sortTyp == SortTyp::WEICHE) {
        exitPointDefaultLeaveState();
        new(this) MotorLaeuft2;
        entryPointDefaultEnter();
        return TriggerProcessingState::consumed;
    }
    return TriggerProcessingState::pending;
}