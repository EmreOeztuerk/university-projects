/**
 * Trigger und Aktionen des Zustands "WerkstueckLaeuft6".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "werkstueck_laeuft6.h"
#include "../hs_zustand/hs_zustand4.h"
#include "../metallsensor_bereich/metallsensor_bereich4.h"

void WerkstueckLaeuft6::entry() {
    action->enteredWerkstueckLaeuft6();
    data->zeitStarten();
}

TriggerProcessingState WerkstueckLaeuft6::s_lsh_ub() {
    cout << "Service Mode BCKalibrierung WerkstueckLaeuft6: lsh_ub" << endl;

    exitPointDefaultLeaveState();
    new(this) HSzustand4;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}

TriggerProcessingState WerkstueckLaeuft6::s_metal_an() {
    cout << "Service Mode BCKalibrierung WerkstueckLaeuft6: ms_an" << endl;

    exitPointDefaultLeaveState();
    new(this) MetallsensorBereich4;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}