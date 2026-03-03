/**
 * Trigger und Aktionen des Zustands "MetallsensorBereich2".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "metallsensor_bereich2.h"
#include "../aussortierungsbereich/aussortierungsbereich2.h"

void MetallsensorBereich2::entry() {
    action->enteredMetallsensorBereich2();
    //data->zeitSpeichern(data->t_lsh_lsm, "");
}

TriggerProcessingState MetallsensorBereich2::s_metal_aus() {
    cout << "Service Mode BCKalibrierung MetallsensorBereich2: ms_aus" << endl;

    exitPointDefaultLeaveState();
    new(this) Aussortierungsbereich2;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}