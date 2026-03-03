/**
 * Trigger und Aktionen des Zustands "WerkstueckLaeuft".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "werkstueck_laeuft.h"
#include "../hs_zustand/hs_zustand.h"
#include "../metallsensor_bereich/metallsensor_bereich.h"

void WerkstueckLaeuft::entry() {
    action->enteredWerkstueckLaeuft();
    data->zeitStarten();
}

TriggerProcessingState WerkstueckLaeuft::s_lsh_ub() {
    cout << "Service Mode BCKalibrierung WerkstueckLaeuft: lsh_ub" << endl;

    exitPointDefaultLeaveState();
    new(this) HSzustand;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}

TriggerProcessingState WerkstueckLaeuft::s_metal_an() {
    cout << "Service Mode BCKalibrierung WerkstueckLaeuft: ms_an" << endl;

    exitPointDefaultLeaveState();
    new(this) MetallsensorBereich;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}