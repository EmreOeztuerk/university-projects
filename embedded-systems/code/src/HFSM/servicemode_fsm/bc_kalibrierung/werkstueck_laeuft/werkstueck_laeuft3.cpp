/**
 * Trigger und Aktionen des Zustands "WerkstueckLaeuft3".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "werkstueck_laeuft3.h"
#include "../hs_zustand/hs_zustand2.h"
#include "../metallsensor_bereich/metallsensor_bereich2.h"

void WerkstueckLaeuft3::entry() {
    action->enteredWerkstueckLaeuft3();
    data->zeitStarten();
}

TriggerProcessingState WerkstueckLaeuft3::s_lsh_ub() {
    cout << "Service Mode BCKalibrierung WerkstueckLaeuft3: lsh_ub" << endl;

    exitPointDefaultLeaveState();
    new(this) HSzustand2;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}

TriggerProcessingState WerkstueckLaeuft3::s_metal_an() {
    cout << "Service Mode BCKalibrierung WerkstueckLaeuft3: ms_an" << endl;

    exitPointDefaultLeaveState();
    new(this) MetallsensorBereich2;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}