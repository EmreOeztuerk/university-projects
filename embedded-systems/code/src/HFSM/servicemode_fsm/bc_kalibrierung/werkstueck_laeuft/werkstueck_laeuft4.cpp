/**
 * Trigger und Aktionen des Zustands "WerkstueckLaeuft4".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "werkstueck_laeuft4.h"
#include "../hs_zustand/hs_zustand3.h"
#include "../metallsensor_bereich/metallsensor_bereich3.h"

void WerkstueckLaeuft4::entry() {
    action->enteredWerkstueckLaeuft4();
    data->zeitStarten();
}

TriggerProcessingState WerkstueckLaeuft4::s_lsh_ub() {
    cout << "Service Mode BCKalibrierung WerkstueckLaeuft4: lsh_ub" << endl;

    exitPointDefaultLeaveState();
    new(this) HSzustand3;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}

TriggerProcessingState WerkstueckLaeuft4::s_metal_an() {
    cout << "Service Mode BCKalibrierung WerkstueckLaeuft4: ms_an" << endl;

    exitPointDefaultLeaveState();
    new(this) MetallsensorBereich3;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}