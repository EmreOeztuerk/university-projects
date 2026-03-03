/**
 * Trigger und Aktionen des Zustands "MetallsensorBereich".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "metallsensor_bereich.h"
#include "../aussortierungsbereich/aussortierungsbereich.h"

void MetallsensorBereich::entry() {
    action->enteredMetallsensorBereich();
    //data->zeitSpeichern(data->t_lsh_lsm, "");
}

TriggerProcessingState MetallsensorBereich::s_metal_aus() {
    cout << "Service Mode BCKalibrierung MetallsensorBereich: ms_aus" << endl;

    exitPointDefaultLeaveState();
    new(this) Aussortierungsbereich;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}